#include "semantic.h"

void                    semantic_line_linked_element(
                        list_t **linked_elements,
                        GstElement *element,
                        char *element_name) {

    linked_element_t    *linked_element = (linked_element_t *)malloc(sizeof(linked_element_t));
    list_t              *new = NULL;

    linked_element->src = element;
    linked_element->caps = NULL;
    linked_element->dst_name = element_name;
    new = lstnew(linked_element, sizeof(linked_element_t));
    if (new == NULL)
        return ;
    lstaddlast(linked_elements, new);
}
