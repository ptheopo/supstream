#include "request.h"

char        *request_show(
            const cJSON *request_json,
            ast_tree_t **root) {


    list_t  *deepblock = ast_deepblock_create(1, "document");
    cJSON   *root_json = ast_node_tojson(*root, deepblock);
    cJSON   *result_json = cJSON_CreateObject();
    char    *result_str = NULL;

    g_print("[REQUEST] Show document\n");
    /* Insert request */
    cJSON_AddItemToObject(result_json, "request", (cJSON *)request_json);
    /* Insert response */
    cJSON_AddItemToObject(result_json, "response", root_json);
    result_str = cJSON_Print(result_json);
    ast_deepblock_free(deepblock);
    return (result_str);
}

char        *request_show_pipeline(
            const cJSON *request_json,
            ast_tree_t **root) {

    cJSON   *target_pipeline = cJSON_GetObjectItemCaseSensitive(request_json, "pipeline");
    list_t  *deepblock = ast_deepblock_create(3, "document", "pipelines", target_pipeline->valuestring);
    cJSON   *root_json = ast_node_tojson(*root, deepblock);
    cJSON   *result_json = cJSON_CreateObject();
    char    *result_str = NULL;

    g_print("[REQUEST] Show pipeline\n");
    /* Insert request */
    cJSON_AddItemToObject(result_json, "request", (cJSON *)request_json);
    /* Insert response */
    cJSON_AddItemToObject(result_json, "response", root_json);
    result_str = cJSON_Print(result_json);
    ast_deepblock_free(deepblock);
    return (result_str);
}