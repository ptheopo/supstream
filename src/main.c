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

static int              core(const gchar *yaml_pipeline) {

    FILE                *file = NULL;
    yaml_parser_t       parser;
    yaml_token_t        token;
    gboolean            done = FALSE;
    fifo_t              *lines = NULL;
    list_t              *deepblock = NULL;
    ast_tree_t          *root = NULL;
    supstream_t         *supstream = NULL;

    pev_init();

    g_print(INIT_OPEN_YAML_O, yaml_pipeline);
    file = fopen(yaml_pipeline, "r");
    if (file == NULL)
        return (FAILED_OPEN_YPIP_OPS);
    if (!yaml_parser_initialize(&parser))
        return (FAILED_INIT_YPIP_OPS);
    yaml_parser_set_input_file(&parser, file);
    while (done != TRUE) {
        yaml_parser_scan(&parser, &token);
        done = parse_token(&token, &root, &lines, &deepblock);
        yaml_token_delete(&token);
    }
    if (root != NULL) {
        supstream = semantic(&root);
        pipeline(supstream);
    }
    free(root);
    //ast_free(&root);
    yaml_parser_delete(&parser);
    assert(!fclose(file));
    return (SUCCESS_OPS);
}

int                     main(int argc, char **argv) {

    int                 ret;

    setlocale(LC_ALL, "");
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
