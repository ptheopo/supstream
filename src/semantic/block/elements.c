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
    ast_node_t          *tmp = *node;
    ast_node_t          *properties = NULL;
    ast_node_t          *pads = NULL;
    ast_node_t          *pad_prop = NULL;
    ast_node_t          *caps = NULL;
    GstElement          *element = NULL;
    /* specify props block inside all elements */
    list_t              *props_dp = lstnew("properties", sizeof(char) * 11);
    list_t              *pads_dp = lstnew("pad_link", sizeof(char) * 9);
    list_t              *pad_prop_deepblock = lstnew("pad_prop", sizeof(char) * 9);
    list_t              *caps_dp = lstnew("caps", sizeof(char) * 5);
    list_t              *linked_elements = NULL;
    list_t              *linked_pads = NULL;
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
            
            /* pad prob */
            pad_prop = ast_iblock_get(
                    (*node)->left,
                    pad_prop_deepblock);
            if (pads != NULL)
                semantic_block_pad_prob(pad_prob, element);


            /* create linked_element list */
            scalar_node_link = ast_iscalar_get_by_key(*node, "element_link");
            if (scalar_node_link != NULL) {
                semantic_line_linked_element(&linked_elements, element, scalar_node_link->right->str);
            } else {
                semantic_line_linked_element(&linked_elements, element, NULL);
            }

            /* create caps */
            caps = ast_iblock_get(
                    (*node)->left,
                    caps_dp);
            if (caps != NULL)
                semantic_block_caps(&linked_elements, caps);

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

    return (result);
}

