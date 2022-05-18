#include "request.h"

char                    *request_create_element(
                        const cJSON *request_json,
                        ast_tree_t **root) {

    (void)request_json;
    (void)root;
    cJSON               *pipeline_json = cJSON_GetObjectItemCaseSensitive(request_json, "pipeline");
    cJSON               *element_json = cJSON_GetObjectItemCaseSensitive(request_json, "element");
    ast_node_t          *new_element = ast_node_jsonstr_toast((cJSON *)element_json);
    list_t              *deepblock = ast_deepblock_create(3, "document", "pipelines", pipeline_json->valuestring);
    list_t              *deepblock_elem = ast_deepblock_create(4, "document", "pipelines", pipeline_json->valuestring, "elements");
    ast_node_t          **pipeline = aast_iblock_get(root, deepblock);
    ast_node_t          **elements = aast_iblock_get(root, deepblock_elem);
    list_t              *linked_elements = NULL;
    list_t              *linked_pads = NULL;
    list_t              *linked_pad_props_lst = NULL;
    semantic_data_t     *sdata_ptr = NULL;

    /* Verify */
    if (deepblock == NULL
            || pipeline == NULL
            || *pipeline == NULL
            || new_element == NULL) {
        return ("{\"code\": \"1\"}");
    }

    g_print("Create element\n");

    /* Semantic Analysis */
    sdata_ptr = (*pipeline)->sdata;
    semantic_block_element(
            &((*pipeline)->symtable),
            sdata_ptr->gstpipeline,
            &new_element,
            &linked_elements,
            &linked_pads,
            &linked_pad_props_lst);

    /* Linking */
    semantic_link(
            &((*pipeline)->symtable),
            &linked_elements,
            &linked_pads,
            &linked_pad_props_lst);

    /* Update pipeline */
    ast_ilb_add_simple(
            elements, // <==== left doesnt work ???
            new_element);

    /* Don't forget to free deepblock */
    /* Don't forget to free deepblock_elem */

    return ("{\"code\": \"0\"}");
}

char                    *request_remove_element(
                        const cJSON *request_json,
                        ast_tree_t **root) {

    cJSON               *pipeline_json = cJSON_GetObjectItemCaseSensitive(request_json, "pipeline");
    cJSON               *element_json = cJSON_GetObjectItemCaseSensitive(request_json, "element");
    t_ast_action_res    res;


    if (cJSON_IsString(pipeline_json) == FALSE || cJSON_IsString(element_json) == FALSE) {
        g_print("Bad JSON format\n");
        return (NULL);
    }

    /* TMP debug */
    res = ast_action_element_remove(root, pipeline_json->valuestring, element_json->valuestring);
    if (res == ACTION_PARAMETERS_FAIL)
        g_print("Bad JSON format\n");
    else if (res == ACTION_PIPELINE_NOT_FOUND)
        g_print("Pipeline not found\n");
    else if (res == ACTION_ELEMENT_IN_PIPELINE_NOT_FOUND)
        g_print("Element in pipeline not found\n");

    return ("{\"ok\": \"ok\"}");
}
