#include "semantic.h"

static void             free_props_block(void *content, size_t size) {
    (void)size;
    free(content);
}

linked_result_t         *semantic_block_elements(
        ast_node_t      **node,
        GHashTable      **symtable,
        GstPipeline     *pipeline) {

    ast_node_t          *scalar_node = NULL;
    ast_node_t          *scalar_node_link = NULL;
    ast_node_t          *scalar_node_start_playing = NULL;
    ast_node_t          *scalar_node_start_paused = NULL;
    ast_node_t          *scalar_node_start_ready = NULL;
    ast_node_t          *scalar_node_start_null = NULL;
    ast_node_t          *tmp = *node;
    ast_node_t          *properties = NULL;
    ast_node_t          *pads = NULL;
    ast_node_t          *pad_props = NULL;
    ast_node_t          *caps = NULL;
    GstElement          *element = NULL;
    /* specify props block inside all elements */
    list_t              *props_dp = lstnew("properties", sizeof(char) * 11);
    list_t              *pads_dp = lstnew("pad_link", sizeof(char) * 9);
    list_t              *pad_props_dp = lstnew("pad_props", sizeof(char) * 10);
    list_t              *caps_dp = lstnew("caps", sizeof(char) * 5);
    list_t              *linked_elements = NULL;
    list_t              *linked_pads = NULL;
    list_t              *pad_props_lst = NULL;
    linked_result_t     *result = (linked_result_t *)malloc(sizeof(linked_result_t));

    if (result == NULL)
        exit(1);
    if ((*node)->left == NULL)
        return (NULL);

    /* move on first element */
    *node = (*node)->left;

    /* for each elements */
    do {

        if (ast_node_is_iblock(*node) == TRUE) {
            /* re-init for each elements */
            scalar_node = NULL;
            scalar_node_link = NULL;
            properties = NULL;
            pads = NULL;
            caps = NULL;
            element = NULL;

            /* obtain element scalar node */
            scalar_node = ast_iscalar_get_by_key(*node, "element");

            /* create new element */
            if (scalar_node == NULL)
                exit_block(node);
            element = (GstElement *)gst_element_factory_make(
                    (const gchar *)scalar_node->right->str,
                    (const gchar *)(*node)->str);
            if (element == NULL)
                exit_block(node);
            g_print(SEMANTIC_NEW_ELEMENT_O, (*node)->str, scalar_node->right->str);

            /* alloc sdata union */
            (*node)->sdata = (semantic_data_t *)malloc(sizeof(semantic_data_t));
            if ((*node)->sdata == NULL)
                exit_block(node);
            (*node)->sdata->gstelement = element;

            /* create properties */
            properties = ast_iblock_get(
                    (*node)->left,
                    props_dp);
            if (properties != NULL)
                semantic_block_properties(properties, element);

            /* create bin */
            gst_bin_add(GST_BIN (pipeline), element);
            g_print(SEMANTIC_ADD_BIN_O,
                    scalar_node->right->str,
                    GST_OBJECT_NAME (pipeline));

            /* create caps */
            caps = ast_iblock_get(
                    (*node)->left,
                    caps_dp);
            if (caps != NULL)
                semantic_block_caps(&linked_elements, caps);

            /* start options (update states from scheduler) */
            scalar_node_start_playing = ast_iscalar_get_by_key(*node, "start_playing");
            if (scalar_node_start_playing != NULL) {
                semantic_line_start_playing(element, scalar_node_start_playing->right->str);
            }
            scalar_node_start_paused = ast_iscalar_get_by_key(*node, "start_paused");
            if (scalar_node_start_paused != NULL) {
                semantic_line_start_paused(element, scalar_node_start_paused->right->str);
            }
            scalar_node_start_ready = ast_iscalar_get_by_key(*node, "start_ready");
            if (scalar_node_start_ready != NULL) {
                semantic_line_start_ready(element, scalar_node_start_ready->right->str);
            }
            scalar_node_start_null = ast_iscalar_get_by_key(*node, "start_null");
            if (scalar_node_start_null != NULL) {
                semantic_line_start_null(element, scalar_node_start_null->right->str);
            }
            
            /* create linked_element list */
            scalar_node_link = ast_iscalar_get_by_key(*node, "element_link");
            if (scalar_node_link != NULL) {
                semantic_line_linked_element(&linked_elements, element, scalar_node_link->right->str);
            } else {
                semantic_line_linked_element(&linked_elements, element, NULL);
            }

            /* create pad_props list */
            pad_props = ast_iblock_get(
                    (*node)->left,
                    pad_props_dp);
            if (pad_props != NULL)
                semantic_block_pad_props(&pad_props_lst, pad_props, element);

            /* create linked_pad list */
            pads = ast_iblock_get(
                    (*node)->left,
                    pads_dp);
            if (pads != NULL)
                semantic_block_pads(&linked_pads, pads, element);

            /* create element sym in symtable */
            g_hash_table_insert (*symtable, (*node)->str, element);
        }

        *node = (*node)->right;

    } while (*node != NULL);

    lstdel(&props_dp, &free_props_block);
    lstdel(&pads_dp, &free_props_block);
    lstdel(&caps_dp, &free_props_block);
    *node = tmp;
    result->elements = linked_elements;
    result->pads = linked_pads;
    result->pad_props = pad_props_lst;

    return (result);
}

