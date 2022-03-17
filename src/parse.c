#include "parse.h"

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

gboolean                core_token_parse(
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
