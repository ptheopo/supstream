#include "link.h"

void                    link_element_all(list_t *linked_elements) {

    list_t              *tmp = linked_elements;
    list_t              *next = NULL;
    linked_element_t    *content = NULL;
    linked_element_t    *content_tmp = NULL;
    gboolean            ret = FALSE;

    if (linked_elements) {
        while (linked_elements) {
            content = (linked_element_t *)linked_elements->content;
            while (tmp) {
                content_tmp = (linked_element_t *)tmp->content;
                if (content
                        && content_tmp
                        && content->dst_name
                        && content_tmp->src
                        && strcmp(content->dst_name, GST_OBJECT_NAME (content_tmp->src)) == 0) {

                    /* link elements with(out) caps */
                    if (content->caps != NULL) {
                        ret = gst_element_link_filtered(content->src,
                                content_tmp->src,
                                content->caps);
                    } else {
                        ret = gst_element_link(content->src, content_tmp->src);
                    }

                    /* show result */
                    if (ret == FALSE) {
                        g_printerr(SEMANTIC_ERROR_LINKED_ELEMENT_O,
                                GST_OBJECT_NAME (content->src),
                                GST_OBJECT_NAME (content_tmp->src));
                    } else {
                        g_print(SEMANTIC_LINKED_ELEMENT_O,
                                GST_OBJECT_NAME (content->src),
                                GST_OBJECT_NAME (content_tmp->src));
                    }
                    break ;
                }
                tmp = tmp->next;
            }
            next = linked_elements->next;
            linked_elements = next;
            tmp = linked_elements;
        }
    }
}
