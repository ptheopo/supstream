#include "request.h"

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
                free((*element_link)->right->str);
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
