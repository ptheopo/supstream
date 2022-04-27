#include "request.h"

int                         state_update(ast_node_t **pipeline, char *state_str) {

    GstStateChangeReturn    state_ret;
    list_t                  *deepblock = ast_deepblock_create(1, (*pipeline)->str);
    ast_node_t              **state = aast_iscalar_get_by_key(pipeline, "init_state");
    ast_node_t              *key = NULL;
    ast_node_t              *value = NULL;
    ast_node_t              *iscalar = NULL;
    ast_node_t              *iline = NULL;

    if (state_str == NULL)
        return (1);

    /* Use binary flags with switch */
    if (STATE_IS_READY(state_str)) {
        state_ret = gst_element_set_state(
                GST_ELEMENT ((*pipeline)->sdata->gstpipeline),
                GST_STATE_READY);
    } else if (STATE_IS_PAUSE(state_str)) {
        state_ret = gst_element_set_state(
                GST_ELEMENT ((*pipeline)->sdata->gstpipeline),
                GST_STATE_PAUSED);
    } else if (STATE_IS_PLAY(state_str)) {
        state_ret = gst_element_set_state(
                GST_ELEMENT ((*pipeline)->sdata->gstpipeline),
                GST_STATE_PLAYING);
    } else if (STATE_IS_NULL(state_str)) {
        state_ret = gst_element_set_state(
                GST_ELEMENT ((*pipeline)->sdata->gstpipeline),
                GST_STATE_NULL);
    }

    if (state_ret == GST_STATE_CHANGE_FAILURE)
        return (1);

    if (state == NULL || *state == NULL) {

        key = ast_node_new(g_strdup("init_state"), iKEY);
        value = ast_node_new(g_strdup(state_str), iVALUE);
        iscalar = ast_iscalar_new(key, value);
        iline = ast_iline_new(iscalar);
        ast_ilb_add(pipeline, iline, deepblock);

    } else {

        free((*state)->right->str);
        (*state)->right->str = g_strdup(state_str);

    }

    return (0);

}

static int                  request_state_manage(
                            const cJSON *request_json,
                            ast_tree_t  **root,
                            char *state_str,
                            const char  *log_str) {

    list_t                  *deepblock = ast_deepblock_create(2, "document", "pipelines");
    list_t                  *deepblock_pipeline = NULL;
    ast_node_t              **node = &((*aast_iblock_get(root, deepblock))->left);
    ast_node_t              **node_pipeline = &((*aast_iblock_get(root, deepblock))->left);
    cJSON                   *target_pipelines = cJSON_GetObjectItemCaseSensitive(request_json, "pipelines");
    ast_node_t              *node_ptr = *node;
    cJSON                   *target_pipeline = NULL;
    int                     ret = 1;
    /* Only for bypassing -Wformat-security */
    int                     (*printf_bypass_fmt_sec)(const char * restrict,  ...) = printf;

    /* Verify formats */

    if (target_pipelines == NULL) {

        /* All pipelines */

        if (node == NULL || *node == NULL) {

            /* No pipelines launched */
            return (ret);

        }

        printf_bypass_fmt_sec(log_str);
        while (*node != NULL) {
            g_print(REQUEST_GUESS_SUCCESS_O, (*node)->str);
            ret = state_update(node, state_str);
            *node = (*node)->right;
        }
        *node = node_ptr;

    } else {

        /* Multiple or only one pipeline */

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
            node_pipeline = aast_iblock_get(node, deepblock_pipeline);
            if (node_pipeline == NULL || *node_pipeline == NULL) {

                /* Warning, unknown pipeline */
                g_printerr(REQUEST_GUESS_WARNING_O,
                        target_pipeline->valuestring,
                        "unknown pipeline");

            } else {

                /* Pipeline exist */
                ret = state_update(node_pipeline, state_str);
                g_print(REQUEST_GUESS_SUCCESS_O, target_pipeline->valuestring);

            }
            ast_deepblock_free(deepblock_pipeline);
        }

    }
    ast_deepblock_free(deepblock);
    return (ret);
}

char                        *request_state_json_str(const cJSON *request_json, int ret) {

    cJSON                   *result_json = cJSON_CreateObject();
    cJSON                   *response_json = cJSON_CreateObject();
    cJSON                   *code_json = NULL;
    char                    *result_str = NULL;

    cJSON_AddItemToObject(result_json, "request", (cJSON *)request_json);
    code_json = cJSON_CreateNumber(ret);
    cJSON_AddItemToObject(response_json, "code", code_json);
    cJSON_AddItemToObject(result_json, "response", response_json);
    result_str = cJSON_Print(result_json);
    return (result_str);

}

char                        *request_pause(
                            const cJSON *request_json,
                            ast_tree_t **root) {

    int                     ret = 1;
    char                    *result_json_str = NULL;

    ret = request_state_manage(
            request_json,
            root,
            "pause",
            REQUEST_PAUSE_SUCCESS_O);
    result_json_str = request_state_json_str(request_json, ret);
    return (result_json_str);
}

char                        *request_play(
                            const cJSON *request_json,
                            ast_tree_t **root) {

    int                     ret = 1;
    char                    *result_json_str = NULL;

    ret = request_state_manage(
            request_json,
            root,
            "play",
            REQUEST_PLAY_SUCCESS_O);
    result_json_str = request_state_json_str(request_json, ret);
    return (result_json_str);
}

char                        *request_ready(
                            const cJSON *request_json,
                            ast_tree_t **root) {

    int                     ret = 1;
    char                    *result_json_str = NULL;

    ret = request_state_manage(
            request_json,
            root,
            "ready",
            REQUEST_READY_SUCCESS_O);
    result_json_str = request_state_json_str(request_json, ret);
    return (result_json_str);
}

char                        *request_null(
                            const cJSON *request_json,
                            ast_tree_t **root) {

    int                     ret = 1;
    char                    *result_json_str = NULL;

    ret = request_state_manage(
            request_json,
            root,
            "null",
            REQUEST_NULL_SUCCESS_O);
    result_json_str = request_state_json_str(request_json, ret);
    return (result_json_str);
}
