#include "request.h"

char    *request_create_element(
        const cJSON *request_json,
        ast_tree_t **root) {

    (void)request_json;
    (void)root;
    g_print("Create element");
    return (NULL);
}

char    *request_remove_element(
        const cJSON *request_json,
        ast_tree_t **root) {

    (void)request_json;
    (void)root;
    g_print("Remove element");
    return (NULL);
}
