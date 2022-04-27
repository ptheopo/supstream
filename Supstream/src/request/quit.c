#include "request.h"

char    *request_exit(
        const cJSON *request_json,
        ast_tree_t **root) {

    (void)request_json;
    (void)root;
    g_print(REQUEST_EXIT_O);
    exit(0);
    return (NULL);
}
