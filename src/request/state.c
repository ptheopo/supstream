#include "request.h"

static int                  request_pipelines_manage(
                            const cJSON *request_json,
                            ast_tree_t  **root,
                            const char  *log_str,
                            int         (*fn)(ast_node_t **)) {

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
            ret = fn(node);
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
                ret = fn(node_pipeline);
                g_print(REQUEST_GUESS_SUCCESS_O, target_pipeline->valuestring);

            }
            ast_deepblock_free(deepblock_pipeline);
        }

    }
    ast_deepblock_free(deepblock);
    return (ret);
}

static int                  request_pause_fn(ast_node_t **pipeline) {

    GstStateChangeReturn    state_ret;
    list_t                  *deepblock = ast_deepblock_create(1, (*pipeline)->str);
    ast_node_t              **state = aast_iscalar_get_by_key(pipeline, "init_state");
    ast_node_t              *key = NULL;
    ast_node_t              *value = NULL;
    ast_node_t              *iscalar = NULL;
    ast_node_t              *iline = NULL;

    if (state == NULL || *state == NULL) {

        key = ast_node_new(g_strdup("init_state"), iKEY);
        value = ast_node_new(g_strdup("pause"), iVALUE);
        iscalar = ast_iscalar_new(key, value);
        iline = ast_iline_new(iscalar);

        ast_ilb_add(pipeline, iline, deepblock);

    } else {

        state_ret = gst_element_set_state(
                GST_ELEMENT ((*pipeline)->sdata->gstpipeline),
                GST_STATE_PAUSED);
        if (state_ret == GST_STATE_CHANGE_FAILURE)
            return (1);
        free((*state)->right->str);
        (*state)->right->str = g_strdup("pause");

    }
    return (0);
}

char                        *request_pause(
                            const cJSON *request_json,
                            ast_tree_t **root) {

    cJSON                   *result_json = cJSON_CreateObject();
    cJSON                   *response_json = cJSON_CreateObject();
    cJSON                   *code_json = NULL;
    char                    *result_str = NULL;
    int                     ret = 1;

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

static int                  request_play_fn(ast_node_t **pipeline) {

    GstStateChangeReturn    state_ret;
    list_t                  *deepblock = ast_deepblock_create(1, (*pipeline)->str);
    ast_node_t              **state = aast_iscalar_get_by_key(pipeline, "init_state");
    ast_node_t              *key = NULL;
    ast_node_t              *value = NULL;
    ast_node_t              *iscalar = NULL;
    ast_node_t              *iline = NULL;

    if (state == NULL || *state == NULL) {

        key = ast_node_new(g_strdup("init_state"), iKEY);
        value = ast_node_new(g_strdup("play"), iVALUE);
        iscalar = ast_iscalar_new(key, value);
        iline = ast_iline_new(iscalar);

        ast_ilb_add(pipeline, iline, deepblock);

    } else {

        state_ret = gst_element_set_state(
                GST_ELEMENT ((*pipeline)->sdata->gstpipeline),
                GST_STATE_PLAYING);
        if (state_ret == GST_STATE_CHANGE_FAILURE)
            return (1);
        free((*state)->right->str);
        (*state)->right->str = g_strdup("play");

    }

    return (0);
}

char                        *request_play(
                            const cJSON *request_json,
                            ast_tree_t **root) {

    cJSON                   *result_json = cJSON_CreateObject();
    cJSON                   *response_json = cJSON_CreateObject();
    cJSON                   *code_json = NULL;
    char                    *result_str = NULL;
    int                     ret = 1;

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

static int                  request_ready_fn(ast_node_t **pipeline) {

    GstStateChangeReturn    state_ret;
    list_t                  *deepblock = ast_deepblock_create(1, (*pipeline)->str);
    ast_node_t              **state = aast_iscalar_get_by_key(pipeline, "init_state");
    ast_node_t              *key = NULL;
    ast_node_t              *value = NULL;
    ast_node_t              *iscalar = NULL;
    ast_node_t              *iline = NULL;

    if (state == NULL || *state == NULL) {

        key = ast_node_new(g_strdup("init_state"), iKEY);
        value = ast_node_new(g_strdup("ready"), iVALUE);
        iscalar = ast_iscalar_new(key, value);
        iline = ast_iline_new(iscalar);

        ast_ilb_add(pipeline, iline, deepblock);

    } else {

        state_ret = gst_element_set_state(
                GST_ELEMENT ((*pipeline)->sdata->gstpipeline),
                GST_STATE_READY);
        if (state_ret == GST_STATE_CHANGE_FAILURE)
            return (1);
        free((*state)->right->str);
        (*state)->right->str = g_strdup("ready");

    }

    return (0);
}

char                        *request_ready(
                            const cJSON *request_json,
                            ast_tree_t **root) {

    cJSON                   *result_json = cJSON_CreateObject();
    cJSON                   *response_json = cJSON_CreateObject();
    cJSON                   *code_json = NULL;
    char                    *result_str = NULL;
    int                     ret = 1;

    ret = request_pipelines_manage(
            request_json,
            root,
            REQUEST_READY_SUCCESS_O,
            &request_ready_fn);

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

static int                  request_null_fn(ast_node_t **pipeline) {

    GstStateChangeReturn    state_ret;
    list_t                  *deepblock = ast_deepblock_create(1, (*pipeline)->str);
    ast_node_t              **state = aast_iscalar_get_by_key(pipeline, "init_state");
    ast_node_t              *key = NULL;
    ast_node_t              *value = NULL;
    ast_node_t              *iscalar = NULL;
    ast_node_t              *iline = NULL;

    if (state == NULL || *state == NULL) {

        key = ast_node_new(g_strdup("init_state"), iKEY);
        value = ast_node_new(g_strdup("null"), iVALUE);
        iscalar = ast_iscalar_new(key, value);
        iline = ast_iline_new(iscalar);

        ast_ilb_add(pipeline, iline, deepblock);

    } else {

        state_ret = gst_element_set_state(
                GST_ELEMENT ((*pipeline)->sdata->gstpipeline),
                GST_STATE_NULL);
        if (state_ret == GST_STATE_CHANGE_FAILURE)
            return (1);

        free((*state)->right->str);
        (*state)->right->str = g_strdup("null");

    }
    return (0);
}

char                        *request_null(
                            const cJSON *request_json,
                            ast_tree_t **root) {

    cJSON                   *result_json = cJSON_CreateObject();
    cJSON                   *response_json = cJSON_CreateObject();
    cJSON                   *code_json = NULL;
    char                    *result_str = NULL;
    int                     ret = 1;

    ret = request_pipelines_manage(
            request_json,
            root,
            REQUEST_NULL_SUCCESS_O,
            &request_null_fn);

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
