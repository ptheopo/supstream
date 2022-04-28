#include "request.h"

char    *request_create_element(
        const cJSON *request_json,
        ast_tree_t **root) {

    (void)request_json;
    (void)root;

    ast_node_t *tmp = ast_node_jsonstr_toast((cJSON *)request_json);
    ast_browse_prefix(tmp, ast_node_print);
    //supstream_t *sup2 = semantic_doc(&tmp);
    g_print("Create element");
    return ("{\"ok\": \"ok\"}");
}

char            *request_remove_element(
                const cJSON *request_json,
                ast_tree_t **root) {

    cJSON               *pipeline_json = cJSON_GetObjectItemCaseSensitive(request_json, "pipeline");
    cJSON               *element_json = cJSON_GetObjectItemCaseSensitive(request_json, "element");
    t_ast_action_res    res;


    if (cJSON_IsString(pipeline_json) == FALSE || cJSON_IsString(element_json) == FALSE) {
        g_print("Bad JSON format\n");
        return (NULL);
    }

    /* TMP debug */
    res = ast_action_element_remove(root, pipeline_json->valuestring, element_json->valuestring);
    if (res == ACTION_PARAMETERS_FAIL)
        g_print("Bad JSON format\n");
    else if (res == ACTION_PIPELINE_NOT_FOUND)
        g_print("Pipeline not found\n");
    else if (res == ACTION_ELEMENT_IN_PIPELINE_NOT_FOUND)
        g_print("Element in pipeline not found\n");

    return ("{\"ok\": \"ok\"}");
}
