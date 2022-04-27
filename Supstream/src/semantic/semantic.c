#include "semantic.h"

void                    exit_block(ast_node_t **node) {

    g_printerr(SEMANTIC_ERROR_PLUGIN_O, (*node)->str);
    ast_free(node);
    exit(1);
}

static void             linked_free(void *content, size_t size) {
    (void)size;
    free(content);
}

static void             semantic_pipeline(
                        ast_node_t **node,
                        config_pipeline_t *config_pipeline) {

    /* used by semantic_block_elements for gst_add_bin fn */
    linked_result_t     *linked = NULL;
    GstPipeline         *pipeline = NULL;
    ast_node_t          **elements = NULL;
    ast_node_t          *tmp = *node;
    list_t              *elements_dp = lstnew("elements", sizeof(char) * 9);

    semantic_block_pipelines(node);
    pipeline = (*node)->sdata->gstpipeline;

    (*node)->config_pipeline = config_pipeline;

    elements = aast_iblock_get(
            &((*node)->left),
            elements_dp);
    if (elements != NULL) {
        (*node)->symtable = g_hash_table_new (g_str_hash, g_str_equal);
        linked = semantic_block_elements(elements, &((*node)->symtable), pipeline);
        link_element_all(linked->elements);
        link_pad_all_always(&((*node)->symtable), linked->pads);
        link_pad_all_request(&((*node)->symtable), linked->pads);
        link_pad_all_sometimes(&((*node)->symtable), linked->pads);
        link_pad_props(linked->pad_props);
        lstdel(&(linked->elements), &linked_free);
        //lstdel(&(linked->pads), &linked_free);
    }
    *node = tmp;

}

supstream_t             *semantic(ast_tree_t **root) {

    ast_node_t          *tmp = *root;
    ast_node_t          *priority_root = *root;
    ast_node_t          *tmp2 = *root;
    supstream_t         *supstream = (supstream_t *)malloc(sizeof(supstream_t));
    config_t            *config = config_new_default();
    config_pipeline_t   *config_pipeline = NULL;

    *root = (*root)->left;
    priority_root = *root;

    /* Priority blocks (global options) */

    while (priority_root) {
        if (AST_IBLOCK_IS (priority_root, "supstream") && priority_root->left) {
            semantic_config(*root, &config);
        }
        priority_root = priority_root->right;
    }

    /* Apply each pipelines */

    while (*root) {

        tmp2 = *root;
        if (AST_IBLOCK_IS (*root, "elements") && (*root)->left) {
            *root = (*root)->left;
        } else if (AST_IBLOCK_IS (*root, "pipelines") && (*root)->left) {
            *root = (*root)->left;

            while (*root != NULL) {

                if (AST_IS_IBLOCK (*root) == TRUE) {

                    /* Specific pipeline config */

                    config_pipeline = config_new_default_pipeline();
                    semantic_config_pipeline(*root, &config_pipeline);

                    semantic_pipeline(root, config_pipeline);

                    /* delay */

                    semantic_config_set_delay(config_pipeline, (*root)->sdata->gstpipeline);

                }

                *root = (*root)->right;
            }
        }
        *root = tmp2;
        *root = (*root)->right;

    }

    /* End ptr */

    *root = tmp;
    supstream->root = root;
    supstream->config = config;
    return (supstream);

}
