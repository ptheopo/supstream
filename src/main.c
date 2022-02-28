#include "main.h"

char                    getch(void)
{
    char                buf = 0;
    struct termios      old = {0};

    fflush(stdout);
    if(tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if(tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if(read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if(tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    printf("%c\n", buf);
    return buf;
}

void                    sigint(int code) {

    char                buffer = 'n';

    g_print(SIGNAL_SIGINT_O);
    buffer = getch();
    if (buffer == 'y' || buffer == 'Y')
        exit(code);
}

/*
 * This function return the block name of inserted lines for a start block
 * or NULL for end block
 */

static char             *push_fifo_to_ast(fifo_t **lines,
        ast_tree_t      **root,
        list_t          *deepblock) {

    int                 count = 0;
    int                 i = 0;
    ast_node_t          *node_line = NULL;
    ast_node_t          *node_scalar = NULL;
    ast_node_t          *node_key = NULL;
    ast_node_t          *node_value = NULL;
    char                *str_key = NULL;
    char                *str_value = NULL;
    char                *block_name = NULL;
    ast_node_t          *tmp_block = NULL;

    /* for start and end block */
    count = fifo_count(*lines);
    for (i = 0; i < count / 2; i++) {
        /* insert new lines */
        str_key = fifo_pop(lines);
        str_value = fifo_pop(lines);
        node_key = ast_node_new(str_key, iKEY);
        node_value = ast_node_new(str_value, iVALUE);
        node_scalar = ast_iscalar_new(node_key, node_value);
        node_line = ast_iline_new(node_scalar);
        ast_ilb_add(root, node_line, deepblock);
    }
    /* only for start block */
    if (count % 2 != 0) {
        /* insert new block */
        block_name = fifo_pop(lines);
        tmp_block = ast_iblock_new(block_name);
        ast_ilb_add(root, tmp_block, deepblock);
        return (block_name);
    }
    return (NULL);
}

static gboolean         core_token_parse(
        yaml_token_t    *token,
        ast_tree_t      **root,
        fifo_t          **lines,
        list_t          **deepblock) {

    static unsigned int nbline = 1;
    ast_node_t          *tmp_block = NULL;
    char                *block_name = NULL;
    list_t              *lst = NULL;

    switch (token->type) {
        case YAML_STREAM_END_TOKEN:
            return (TRUE);
            break ;
        case YAML_STREAM_START_TOKEN:
            break ;
        case YAML_KEY_TOKEN:
            nbline += 1;
            break ;
        case YAML_VALUE_TOKEN:
            break ;
        case YAML_SCALAR_TOKEN:
            fifo_push(lines, (char *)token->data.scalar.value);
            break ;
        case YAML_BLOCK_MAPPING_START_TOKEN:
            if (*root == NULL) {
                /* init AST */
                tmp_block = ast_iblock_new((char *)strdup("document"));
                *root = tmp_block;
                /* update current block */
                lst = lstnew("document", sizeof(char) * 9);
                lstaddlast(deepblock, lst);
            } else {
                /* insert new lines and new block (last line in fifo) */
                block_name = push_fifo_to_ast(lines, root, *deepblock);
                /* update current block */
                lst = lstnew(block_name, sizeof(char) * (strlen(block_name) + 1));
                lstaddlast(deepblock, lst);
            }
            break ;
        case YAML_BLOCK_END_TOKEN:
            /* insert last existing new lines */
            push_fifo_to_ast(lines, root, *deepblock);
            /* update current block */
            lstdellast(deepblock);
            break ;
        default:
            return (TRUE);
            break ;
    }
    return (FALSE);
}

static int              core(const gchar *yaml_pipeline) {

    FILE                *file = NULL;
    yaml_parser_t       parser;
    yaml_token_t        token;
    gboolean            done = FALSE;
    fifo_t              *lines = NULL;
    list_t              *deepblock = NULL;
    ast_tree_t          *root = NULL;
    supstream_t         *supstream = NULL;

    g_print(INIT_OPEN_YAML_O, yaml_pipeline);
    file = fopen(yaml_pipeline, "r");
    if (file == NULL)
        return (FAILED_OPEN_YPIP_OPS);
    if (!yaml_parser_initialize(&parser))
        return (FAILED_INIT_YPIP_OPS);
    yaml_parser_set_input_file(&parser, file);
    while (done != TRUE) {
        yaml_parser_scan(&parser, &token);
        done = core_token_parse(&token, &root, &lines, &deepblock);
        yaml_token_delete(&token);
    }
    if (root != NULL) {
        supstream = semantic_apply(&root);
        exec_pipeline(supstream);
    }
    free(root);
    //ast_free(&root);
    yaml_parser_delete(&parser);
    assert(!fclose(file));
    return (SUCCESS_OPS);
}

int                     main(int argc, char **argv) {

    int                 ret;

    signal(SIGINT, &sigint);

    g_print(INIT_GSTREAMER_O);

    gst_init(&argc, &argv);
    if (argc == 2) {
        ret = core((const gchar *)argv[1]);
        switch (ret) {
            case FAILED_OPEN_YPIP_OPS:
                g_printerr("supstream: error: can't open YAML pipeline\n");
                break ;
            case FAILED_INIT_YPIP_OPS:
                g_printerr("supstream: error: can't load YAML pipeline\n");
                break ;
            default:
                break ;
        }
    } else {
        g_print("usage: supstream <yaml-pipeline>\n");
    }
    return (0);
}
