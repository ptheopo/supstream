#ifndef LINKED_H
# define LINKED_H

# include "list.h"
# include "def.h"

# include <gst/gst.h>
# include <gst/gstelement.h>
# include <glib.h>

/*
 * This functions are used for linking ops with :
 * - gst_element_link
 * - gst_element_link_filtered
 * - gst_pad_link
 * Apply on pads and elements
 */

typedef struct      linked_element_s {
    GstCaps         *caps;
    GstElement      *src;
    char            *dst_name;
}                   linked_element_t;

typedef struct      linked_pad_s {
    GstElement      *src_element;
    char            *src_pad_name;
    char            *dst_element_name;
    char            *dst_pad_name;
}                   linked_pad_t;

typedef struct      linked_result_s {
    list_t          *elements;
    list_t          *pads;
}                   linked_result_t;

typedef struct      link_data_s {
    list_t          *linked_pads;
    GHashTable      **symtable;
}                   link_data_t;

void                link_pad_all_always(
                    GHashTable **symtable,
                    list_t *linked_pads);
void                link_pad_all_request(
                    GHashTable **symtable,
                    list_t *linked_pads);
void                link_pad_all_sometimes(
                    GHashTable **symtable,
                    list_t *linked_pads);
void                link_element_all(list_t *linked_elements);

#endif
