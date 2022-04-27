#include "request.h"

char        *request_version(
            const cJSON *request_json,
            ast_tree_t **root) {

    cJSON   *result_json = cJSON_CreateObject();
    cJSON   *response_json = cJSON_CreateObject();
    cJSON   *version_json = cJSON_CreateString(SUPSTREAM_VERSION_STR);
    char    *result_str = NULL;

    (void)request_json;
    (void)root;
    /* Insert request */
    cJSON_AddItemToObject(result_json, "request", (cJSON *)request_json);
    /* Prepare response */
    cJSON_AddItemToObject(response_json, "version", version_json);
    /* Insert response */
    cJSON_AddItemToObject(result_json, "response", response_json);
    result_str = cJSON_Print(result_json);
    return (result_str);
}
