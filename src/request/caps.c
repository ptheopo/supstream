#include "request.h"

char    *request_set_caps(
        const cJSON *request_json,
        ast_tree_t **root) {

    (void)request_json;
    (void)root;
    g_print("Set caps");
    return (NULL);
}
