#include "request.h"

/* Utils fn */

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

        /* All pipelines */

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

/* Linking */

char                *request_unlink_elements(
        const cJSON *request_json,
        ast_tree_t **root) {

    list_t          *deepblock = ast_deepblock_create(2, "document", "pipelines");
    list_t          *deepblock_pipeline = NULL;
    list_t          *deepblock_src = NULL;
    list_t          *deepblock_sink = NULL;
    ast_node_t      **node = &((*aast_iblock_get(root, deepblock))->left);
    ast_node_t      **src_element = NULL;
    ast_node_t      **sink_element = NULL;
    ast_node_t      **node_pipeline = NULL;
    cJSON           *target_pipeline = cJSON_GetObjectItemCaseSensitive(request_json, "pipeline");
    cJSON           *src = cJSON_GetObjectItemCaseSensitive(request_json, "src");
    cJSON           *sink = cJSON_GetObjectItemCaseSensitive(request_json, "sink");
    cJSON           *result_json = cJSON_CreateObject();
    cJSON           *response_json = cJSON_CreateObject();
    cJSON           *code_json = NULL;
    char            *result_str = NULL;

    /* Verify formats */
    if ((target_pipeline == NULL || src == NULL || sink == NULL)
            || (!cJSON_IsString(target_pipeline)
                || !cJSON_IsString(src)
                || !cJSON_IsString(sink))) {

        /* Insert code ret */
        code_json = cJSON_CreateNumber(1);
        cJSON_AddItemToObject(response_json, "code", code_json);

        g_printerr(REQUEST_BAD_FORMAT_O);

    } else {

        deepblock_pipeline = ast_deepblock_create(1, target_pipeline->valuestring);
        node_pipeline = aast_iblock_get(node, deepblock_pipeline);
        if (node_pipeline == NULL || *node_pipeline == NULL) {

            /* Warning, unknown pipeline */
            g_printerr("[ERROR] Unlink element\n");

            /* Insert code ret */
            code_json = cJSON_CreateNumber(1);
            cJSON_AddItemToObject(response_json, "code", code_json);

        } else {

            /* Pipeline exist */
            deepblock_src = ast_deepblock_create(1, src->valuestring);
            deepblock_sink = ast_deepblock_create(1, sink->valuestring);

            src_element = aast_iblock_get(node_pipeline, deepblock_src);
            sink_element = aast_iblock_get(node_pipeline, deepblock_sink);

            gst_element_unlink(
                    (*src_element)->sdata->gstelement,
                    (*sink_element)->sdata->gstelement);

            /* Update root btree */
            ast_node_remove_by_key(src_element, "element_link");

            /* Free useless deepblocks */
            ast_deepblock_free(deepblock_src);
            ast_deepblock_free(deepblock_sink);

            /* Insert code ret */
            code_json = cJSON_CreateNumber(0);
            cJSON_AddItemToObject(response_json, "code", code_json);

            g_print("[SUCCESS] Unlink element\n");

        }
    }

    ast_deepblock_free(deepblock_pipeline);
    ast_deepblock_free(deepblock);

    /* Insert request */
    cJSON_AddItemToObject(result_json, "request", (cJSON *)request_json);
    /* Insert response */
    cJSON_AddItemToObject(result_json, "response", response_json);
    result_str = cJSON_Print(result_json);

    return (result_str);

}

char                *request_link_elements(
        const cJSON *request_json,
        ast_tree_t **root) {

    list_t          *deepblock = ast_deepblock_create(2, "document", "pipelines");
    list_t          *deepblock_pipeline = NULL;
    list_t          *deepblock_src = NULL;
    list_t          *deepblock_sink = NULL;
    ast_node_t      **node = &((*aast_iblock_get(root, deepblock))->left);
    ast_node_t      **src_element = NULL;
    ast_node_t      **sink_element = NULL;
    ast_node_t      **node_pipeline = NULL;
    ast_node_t      **element_link = NULL;
    cJSON           *target_pipeline = cJSON_GetObjectItemCaseSensitive(request_json, "pipeline");
    cJSON           *src = cJSON_GetObjectItemCaseSensitive(request_json, "src");
    cJSON           *sink = cJSON_GetObjectItemCaseSensitive(request_json, "sink");
    cJSON           *result_json = cJSON_CreateObject();
    cJSON           *response_json = cJSON_CreateObject();
    cJSON           *code_json = NULL;
    char            *result_str = NULL;

    /* Verify formats */
    if ((target_pipeline == NULL || src == NULL || sink == NULL)
            || (!cJSON_IsString(target_pipeline)
                || !cJSON_IsString(src)
                || !cJSON_IsString(sink))) {

        /* Insert code ret */
        code_json = cJSON_CreateNumber(1);
        cJSON_AddItemToObject(response_json, "code", code_json);

        g_printerr(REQUEST_BAD_FORMAT_O);

    } else {

        deepblock_pipeline = ast_deepblock_create(1, target_pipeline->valuestring);
        node_pipeline = aast_iblock_get(node, deepblock_pipeline);
        if (node_pipeline == NULL) {

            /* Warning, unknown pipeline */
            g_printerr("[ERROR] Link element\n");

            /* Insert code ret */
            code_json = cJSON_CreateNumber(1);
            cJSON_AddItemToObject(response_json, "code", code_json);

        } else {

            /* Pipeline exist */
            deepblock_src = ast_deepblock_create(1, src->valuestring);
            deepblock_sink = ast_deepblock_create(1, sink->valuestring);

            src_element = aast_iblock_get(node_pipeline, deepblock_src);
            sink_element = aast_iblock_get(node_pipeline, deepblock_sink);

            gst_element_link(
                    (*src_element)->sdata->gstelement,
                    (*sink_element)->sdata->gstelement);

            /* Update root btree */
            element_link = aast_iscalar_get_by_key(src_element, "element_link");
            if (element_link == NULL) {

                ast_node_t *key = ast_node_new(g_strdup("element_link"), iKEY);
                ast_node_t *value = ast_node_new(g_strdup(sink->valuestring), iVALUE);
                ast_node_t *iscalar = ast_iscalar_new(key, value);
                ast_node_t *iline = ast_iline_new(iscalar);

                ast_ilb_add(src_element, iline, deepblock_src);

            } else {

                // /!\ Need to modify linking list in the future
                free((*element_link)->right);
                (*element_link)->right->str = strdup(sink->valuestring);

            }

            /* Free deepblocks */
            ast_deepblock_free(deepblock_src);
            ast_deepblock_free(deepblock_sink);

            /* Insert code ret */
            code_json = cJSON_CreateNumber(0);
            cJSON_AddItemToObject(response_json, "code", code_json);

            g_print("[SUCCESS] Link element\n");

        }
    }

    ast_deepblock_free(deepblock_pipeline);
    ast_deepblock_free(deepblock);

    /* Insert request */
    cJSON_AddItemToObject(result_json, "request", (cJSON *)request_json);
    /* Insert response */
    cJSON_AddItemToObject(result_json, "response", response_json);
    result_str = cJSON_Print(result_json);

    return (result_str);

}

/* Printer */

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

/* Elements */

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

/* Properties */

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

/* Caps */

char                *request_set_caps(
        const cJSON *request_json,
        ast_tree_t **root) {

    (void)request_json;
    (void)root;
    g_print("Set caps");
    return (NULL);
}

/* Infos */

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

/* States */

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

static int          request_ready_fn(GstPipeline *pipeline) {

    GstStateChangeReturn state_ret;

    state_ret = gst_element_set_state(
            GST_ELEMENT (pipeline),
            GST_STATE_READY);
    if (state_ret == GST_STATE_CHANGE_FAILURE)
        return (1);
    return (0);
}

char                *request_ready(
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

static int          request_null_fn(GstPipeline *pipeline) {

    GstStateChangeReturn state_ret;

    state_ret = gst_element_set_state(
            GST_ELEMENT (pipeline),
            GST_STATE_NULL);
    if (state_ret == GST_STATE_CHANGE_FAILURE)
        return (1);
    return (0);
}

char                *request_null(
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

/* Quit */

char                *request_exit(
        const cJSON *request_json,
        ast_tree_t **root) {

    (void)request_json;
    (void)root;
    g_print(REQUEST_EXIT_O);
    exit(0);
    return (NULL);
}
