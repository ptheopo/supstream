#include "link.h"

void                    link_pad_all_request(
                        GHashTable **symtable,
                        list_t *linked_pads) {

    linked_pad_t        *content = NULL;
    GstElement          *sink_element = NULL;
    GstPad              *src_pad_request = NULL;
    GstPad              *sink_pad_request = NULL;
    link_data_t         *data = (link_data_t *)malloc(sizeof(link_data_t));
    GstPadTemplate      *padtemplate = NULL;

    if (data == NULL)
        return ;

    if (linked_pads) {
        while (linked_pads) {


            content = (linked_pad_t *)linked_pads->content;

            /* Get pads */
            src_pad_request = gst_element_request_pad_simple(content->src_element, content->src_pad_name);
            sink_element = g_hash_table_lookup(*symtable, content->dst_element_name);
            sink_pad_request = gst_element_get_static_pad (sink_element, content->dst_pad_name);

            if (src_pad_request != NULL) {

                /* This is request pad */
                padtemplate = gst_pad_get_pad_template(src_pad_request);

                if (padtemplate->presence == GST_PAD_REQUEST) {

                    /* Link */
                    gst_pad_link(src_pad_request, sink_pad_request);
                    g_print(SEMANTIC_LINKED_PADS_REQUEST_O,
                            content->src_pad_name,
                            content->dst_pad_name);

                }

            }

            linked_pads = linked_pads->next;

        }
    }
}

void                    link_pad_all_always(
                        GHashTable **symtable,
                        list_t *linked_pads) {

    linked_pad_t        *content = NULL;
    GstElement          *sink_element = NULL;
    GstPad              *src_pad_always = NULL;
    GstPad              *sink_pad_always = NULL;
    link_data_t         *data = (link_data_t *)malloc(sizeof(link_data_t));
    GstPadTemplate      *padtemplate = NULL;

    if (data == NULL)
        return ;

    if (linked_pads) {
        while (linked_pads) {


            content = (linked_pad_t *)linked_pads->content;

            /* Get pads */
            src_pad_always = gst_element_get_static_pad (content->src_element, content->src_pad_name);
            sink_element = g_hash_table_lookup(*symtable, content->dst_element_name);
            sink_pad_always = gst_element_get_static_pad (sink_element, content->dst_pad_name);

            if (src_pad_always != NULL) {

                padtemplate = gst_pad_get_pad_template(src_pad_always);

                if (padtemplate->presence == GST_PAD_ALWAYS) {

                    /* src pad is an Always pad */
                    if (sink_pad_always == NULL) {

                        sink_pad_always = gst_element_request_pad_simple(sink_element, content->dst_pad_name);

                        /* Always -> Request/Sometimes? pad linking */
                        gst_pad_link(src_pad_always, sink_pad_always);
                        g_print(SEMANTIC_LINKED_PADS_ALWAYS_REQUEST_O,
                            content->src_pad_name,
                            content->dst_pad_name);

                    } else {
                        
                        /* Always -> Always pad linking */
                        gst_pad_link(src_pad_always, sink_pad_always);
                        g_print(SEMANTIC_LINKED_PADS_ALWAYS_O,
                            content->src_pad_name,
                            content->dst_pad_name);

                    }

                }

            }

            linked_pads = linked_pads->next;

        }
    }
}

/* This function use linked_pads list to connect GstElement based pad */

static void             link_pad_added_handler (
                        GstElement *src,
                        GstPad *new_pad,
                        link_data_t *data) {

    linked_pad_t        *linked_pad = NULL;
    GstPad              *sink_pad = NULL;
    GstElement          *sink_element = NULL;
    GHashTable          **symtable = data->symtable;
    list_t              *linked_pads = data->linked_pads;

    linked_pad = (linked_pad_t *)linked_pads->content;

    while (linked_pads) {

        linked_pad = (linked_pad_t *)linked_pads->content;

        if (strcmp(GST_ELEMENT_NAME (src), GST_ELEMENT_NAME (linked_pad->src_element)) == 0
                && strcmp(GST_PAD_NAME (new_pad), linked_pad->src_pad_name) == 0) {

            sink_element = g_hash_table_lookup(*symtable, linked_pad->dst_element_name);
            sink_pad = gst_element_get_static_pad(sink_element, linked_pad->dst_pad_name);
            gst_pad_link (new_pad, sink_pad);

            gst_object_unref (sink_pad);

        }
        linked_pads = linked_pads->next;

    }

}

/* This function verify if element is already connect with a signal */

static gboolean         link_pad_element_is_connect(list_t *element_connect) {

    char                *element_name;

    while (element_connect) {
        element_name = (char *)element_connect->content;
        if (element_name)
            return (TRUE);
        element_connect = element_connect->next;
    }
    return (FALSE);
}

static void             link_pad_element_connect_free(
                        void *content,
                        size_t size) {

    (void)size;
    free(content);
}

void                    link_pad_all_sometimes(
                        GHashTable **symtable,
                        list_t *linked_pads) {

    linked_pad_t        *linked_pad = NULL;
    list_t              *element_connect = NULL;
    list_t              *new_element_connect = NULL;
    list_t              *tmp_linked_pads = linked_pads;
    char                *element_name = NULL;
    /* data need to be free */
    link_data_t         *data = (link_data_t *)malloc(sizeof(link_data_t));
    /* data need to be free */
    
    while (tmp_linked_pads != NULL) {
        linked_pad = (linked_pad_t *)tmp_linked_pads->content;

        if (element_connect == NULL
                || link_pad_element_is_connect(element_connect) == FALSE) {

            element_name = GST_ELEMENT_NAME (linked_pad->src_element);
            new_element_connect = lstnew(
                    element_name,
                    sizeof(char) * (strlen(element_name) + 1));
            if (new_element_connect == NULL)
                return ;

            /* New element signal connection */
            data->linked_pads = linked_pads;
            data->symtable = symtable;
            g_signal_connect(
                    linked_pad->src_element,
                    "pad-added",
                    G_CALLBACK (link_pad_added_handler),
                    data);

            lstaddlast(&element_connect, new_element_connect);

        }

        tmp_linked_pads = tmp_linked_pads->next;
    }

    lstdel(&element_connect, link_pad_element_connect_free);
}
