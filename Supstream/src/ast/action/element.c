#include "ast.h"

t_ast_action_res    ast_action_element_remove(ast_tree_t **root, char *pipeline_name, char *element_name) {

    list_t          *deepblock = NULL;

    /* Verify and obtain pipeline node */

    if (root == NULL || *root == NULL || pipeline_name == NULL || element_name == NULL) {
        return (ACTION_PARAMETERS_FAIL);
    }

    deepblock = ast_deepblock_create(4, "document", "pipelines", pipeline_name, "elements");

    /* Remove element from pipeline */
    ast_iblock_remove_by_key(root, deepblock, element_name);
    
    return (ACTION_SUCCESS);
}
