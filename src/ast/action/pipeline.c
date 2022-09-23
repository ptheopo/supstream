#include "ast.h"

ast_node_t          *ast_action_pipeline_get(ast_tree_t **root, char *pipeline_name) {

    list_t          *deepblock = NULL;
    ast_node_t      *pipeline = NULL;

    /* Verify and obtain pipeline node */

    if (root == NULL || *root == NULL || pipeline_name == NULL) {
        return (NULL);
    }

    deepblock = ast_deepblock_create(3, "document", "pipelines", pipeline_name);
    pipeline = ast_iblock_get(*root, deepblock);
    
    return (pipeline);
}
