#include "request.h"

static int          request_pipelines_manage(
                    const cJSON *request_json,
                    ast_tree_t **root,
                    const char *log_str,
                    int (*fn)(GstPipeline *)) {

    list_t          *deepblock = ast_deepblock_create(2, "document", "pipelines");
    list_t          *deepblock_pipeline = NULL;
    ast_node_t      *node = ast_iblock_get(*root, deepblock)->left;
    ast_node_t      *node_pipeline = ast_iblock_get(*root, deepblock)->left;
    cJSON           *target_pipelines = cJSON_GetObjectItemCaseSensitive(request_json, "pipelines");
    cJSON           *target_pipeline = NULL;
    int             ret = 1;
    /* Only for bypassing -Wformat-security */
    int             (*printf_bypass_fmt_sec)(const char * restrict,  ...) = printf;

    /* Verify formats */
    if (target_pipelines == NULL) {

        /* Pause all pipelines */

        if (node == NULL) {

            /* No pipelines launched */
            return (ret);

        }

        /* Success */
        printf_bypass_fmt_sec(log_str);
        while (node != NULL) {
            g_print(REQUEST_GUESS_SUCCESS_O, node->str);
            ret = fn(node->sdata->gstpipeline);
            node = node->right;
        }

    } else {

        /* Pause multiple or only one pipeline */

        if (!cJSON_IsArray(target_pipelines)) {

            /* Not an array */
            g_printerr(REQUEST_BAD_FORMAT_O);
            return (ret);

        }

        if (cJSON_GetArraySize(target_pipelines) == 0) {

            /* Empty pipelines array */
            g_printerr(REQUEST_BAD_FORMAT_O);
            return (ret);

        }

        /* Success */
        printf_bypass_fmt_sec(log_str);
        cJSON_ArrayForEach(target_pipeline, target_pipelines) {
            deepblock_pipeline = ast_deepblock_create(1, target_pipeline->valuestring);
            node_pipeline = ast_iblock_get(node, deepblock_pipeline);
            if (node_pipeline == NULL) {

                /* Warning, unknown pipeline */
                g_printerr(REQUEST_GUESS_WARNING_O,
                        target_pipeline->valuestring,
                        "unknown pipeline");

            } else {

                /* Pipeline exist */
                ret = fn(node_pipeline->sdata->gstpipeline);
                g_print(REQUEST_GUESS_SUCCESS_O, target_pipeline->valuestring);

            }
            ast_deepblock_free(deepblock_pipeline);
            node_pipeline = NULL;
        }

    }
    ast_deepblock_free(deepblock);
    return (ret);
}

char                *request_show(
                    const cJSON *request_json,
                    ast_tree_t **root) {


    list_t          *deepblock = ast_deepblock_create(1, "document");
    cJSON           *root_json = ast_node_tojson(*root, deepblock);
    cJSON           *result_json = cJSON_CreateObject();
    char            *result_str = NULL;

    g_print("[REQUEST] Show document\n");
    /* Insert request */
    cJSON_AddItemToObject(result_json, "request", (cJSON *)request_json);
    /* Insert response */
    cJSON_AddItemToObject(result_json, "response", root_json);
    result_str = cJSON_Print(result_json);
    ast_deepblock_free(deepblock);
    return (result_str);
}

char                *request_unlink_elements(
                    const cJSON *request_json,
                    ast_tree_t **root) {

    (void)request_json;
    (void)root;
    g_print("Unlink elements");
    return (NULL);
}

char                *request_link_elements(
                    const cJSON *request_json,
                    ast_tree_t **root) {

    (void)request_json;
    (void)root;
    g_print("Link elements");
    return (NULL);
}

char                *request_create_element(
                    const cJSON *request_json,
                    ast_tree_t **root) {

    (void)request_json;
    (void)root;
    g_print("Create element");
    return (NULL);
}

char                *request_remove_element(
                    const cJSON *request_json,
                    ast_tree_t **root) {

    (void)request_json;
    (void)root;
    g_print("Remove element");
    return (NULL);
}

/*char                *request_set_properties(
                    const cJSON *request_json,
                    ast_tree_t **root) {

    char            *property = NULL;
    cJSON           *property_json = cJSON_GetObjectItemCaseSensitive(request_json, "property");
    cJSON           *property_json = cJSON_GetObjectItemCaseSensitive(request_json, "property");

    (void)request_json;
    (void)root;

    g_print(REQUEST_SET_PROP_SUCCESS_O, );
    g_object_set(G_OBJECT (element), property, type.guint64, NULL);
    return (NULL);
}*/

char                *request_set_properties(
                    const cJSON *request_json,
                    ast_tree_t **root) {

    (void)request_json;
    (void)root;
    g_print("Set properties");
    return (NULL);
}

char                *request_set_caps(
                    const cJSON *request_json,
                    ast_tree_t **root) {

    (void)request_json;
    (void)root;
    g_print("Set caps");
    return (NULL);
}

char                *request_version(
                    const cJSON *request_json,
                    ast_tree_t **root) {

    cJSON           *result_json = cJSON_CreateObject();
    cJSON           *response_json = cJSON_CreateObject();
    cJSON           *version_json = cJSON_CreateString(SUPSTREAM_VERSION_STR);
    char            *result_str = NULL;

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

static int          request_pause_fn(GstPipeline *pipeline) {

    GstStateChangeReturn state_ret;

    state_ret = gst_element_set_state(
        GST_ELEMENT (pipeline),
        GST_STATE_PAUSED);
    if (state_ret == GST_STATE_CHANGE_FAILURE)
        return (1);
    return (0);
}

char                *request_pause(
                    const cJSON *request_json,
                    ast_tree_t **root) {

    cJSON           *result_json = cJSON_CreateObject();
    cJSON           *response_json = cJSON_CreateObject();
    cJSON           *code_json = NULL;
    char            *result_str = NULL;
    int             ret = 1;

    ret = request_pipelines_manage(
        request_json,
        root,
        REQUEST_PAUSE_SUCCESS_O,
        &request_pause_fn);
    /* Insert request */
    cJSON_AddItemToObject(result_json, "request", (cJSON *)request_json);
    /* Prepare response */
    code_json = cJSON_CreateNumber(ret);
    cJSON_AddItemToObject(response_json, "code", code_json);
    /* Insert response */
    cJSON_AddItemToObject(result_json, "response", response_json);
    result_str = cJSON_Print(result_json);
    return (result_str);
}

static int          request_play_fn(GstPipeline *pipeline) {

    GstStateChangeReturn state_ret;

    state_ret = gst_element_set_state(
        GST_ELEMENT (pipeline),
        GST_STATE_PLAYING);
    if (state_ret == GST_STATE_CHANGE_FAILURE)
        return (1);
    return (0);
}

char                *request_play(
                    const cJSON *request_json,
                    ast_tree_t **root) {

    cJSON           *result_json = cJSON_CreateObject();
    cJSON           *response_json = cJSON_CreateObject();
    cJSON           *code_json = NULL;
    char            *result_str = NULL;
    int             ret = 1;

    ret = request_pipelines_manage(
        request_json,
        root,
        REQUEST_PLAY_SUCCESS_O,
        &request_play_fn);
    /* Insert request */
    cJSON_AddItemToObject(result_json, "request", (cJSON *)request_json);
    /* Prepare response */
    code_json = cJSON_CreateNumber(ret);
    cJSON_AddItemToObject(response_json, "code", code_json);
    /* Insert response */
    cJSON_AddItemToObject(result_json, "response", response_json);
    result_str = cJSON_Print(result_json);
    return (result_str);
}

char                *request_exit(
                    const cJSON *request_json,
                    ast_tree_t **root) {

    (void)request_json;
    (void)root;
    g_print(REQUEST_EXIT_O);
    exit(0);
    return (NULL);
}
