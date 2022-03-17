#include "semantic.h"

static void             semantic_block_caps_audio(list_t **linked_elements, ast_node_t *node) {

    linked_element_t    *content = NULL;

    char                *media_type = AST_GET_VALUE (node, "media_type");
    char                *format = AST_GET_VALUE (node, "format");
    char                *channels = AST_GET_VALUE (node, "channels");
    char                *block_align = AST_GET_VALUE (node, "block_align");
    char                *layer = AST_GET_VALUE (node, "layer");
    char                *bitrate = AST_GET_VALUE(node, "bitrate");
    char                *layout = AST_GET_VALUE(node, "layout");

    GstCaps *caps = NULL;

    if (media_type == NULL) {
        g_printerr(SEMANTIC_ERROR_MT_CAPS_O);
        exit(1);
    }

    caps = gst_caps_new_empty_simple(media_type);

    if (format != NULL) {
        gst_caps_set_simple(caps, "format", G_TYPE_STRING, (gchar *)format, NULL);
    } else if (channels != NULL) {
        gst_caps_set_simple(caps, "channels", G_TYPE_INT, (gint)atoi(channels), NULL);
    } else if (layout != NULL) {
        gst_caps_set_simple(caps, "layout", G_TYPE_STRING, (gchar *)layout, NULL);
    } else if (block_align != NULL) {
        gst_caps_set_simple(caps, "block_align", G_TYPE_INT, (gint)atoi(block_align), NULL);
    } else if (layer != NULL) {
        gst_caps_set_simple(caps, "layer", G_TYPE_INT, (gint)atoi(layer), NULL);
    } else if (bitrate != NULL) {
        gst_caps_set_simple(caps, "bitrate", G_TYPE_INT, (gint)atoi(bitrate), NULL);
    }

    if (*linked_elements == NULL)
        return ;

    while ((*linked_elements)->next)
        *linked_elements = (*linked_elements)->next;
    
    content = (linked_element_t *)(*linked_elements)->content;
    content->caps = caps;
}

static void             semantic_block_caps_video(list_t **linked_elements, ast_node_t *node) {

    linked_element_t    *content = NULL;

    char                *media_type = AST_GET_VALUE (node, "media_type");
    char                *format = AST_GET_VALUE (node, "format");
    char                *height = AST_GET_VALUE (node, "height");
    char                *framerate = AST_GET_VALUE (node, "framerate");
    char                *max_framerate = AST_GET_VALUE (node, "max_framerate");
    char                *views = AST_GET_VALUE (node, "views");
    char                *interlace_mode = AST_GET_VALUE (node, "interlace_mode");
    char                *chroma_site = AST_GET_VALUE (node, "chroma_site");
    char                *pixel_aspect_ratio = AST_GET_VALUE (node, "pixel_aspect_ratio");
    char                *h263version = AST_GET_VALUE (node, "h263version");
    char                *depth = AST_GET_VALUE (node, "depth");

    GstCaps *caps = NULL;

    if (media_type == NULL) {
        g_printerr(SEMANTIC_ERROR_MT_CAPS_O);
        exit(1);
    }

    caps = gst_caps_new_empty_simple(media_type);

    if (format != NULL) {
        gst_caps_set_simple(caps, "format", G_TYPE_STRING, (gchar *)format, NULL);
    } else if (height != NULL) {
        gst_caps_set_simple(caps, "height", G_TYPE_INT, (gint)atoi(height), NULL);
    } else if (framerate != NULL) {
        //gst_caps_set_simple(caps, "framerate", G_TYPE_FRACTION, (gint)atoi(framerate), NULL);
    } else if (max_framerate != NULL) {
        //gst_caps_set_simple(caps, "max-framerate", G_TYPE_FRACTION, (gint)atoi(max_framerate), NULL);
    } else if (views != NULL) {
        gst_caps_set_simple(caps, "views", G_TYPE_INT, (gint)atoi(views), NULL);
    } else if (interlace_mode != NULL) {
        gst_caps_set_simple(caps, "interlace-mode", G_TYPE_STRING, (gchar *)interlace_mode, NULL);
    } else if (chroma_site != NULL) {
        gst_caps_set_simple(caps, "chroma-site", G_TYPE_STRING, (gchar *)chroma_site, NULL);
    } else if (pixel_aspect_ratio != NULL) {
        //gst_caps_set_simple(caps, "pixel-aspect-ratio", G_TYPE_FRACTION, (gint)atoi(height), NULL);
    } else if (h263version != NULL) {
        gst_caps_set_simple(caps, "h263version", G_TYPE_STRING, (gchar *)h263version, NULL);
    } else if (depth != NULL) {
        gst_caps_set_simple(caps, "depth", G_TYPE_INT, (gint)atoi(depth), NULL);
    }

    if (*linked_elements == NULL)
        return ;

    while ((*linked_elements)->next)
        *linked_elements = (*linked_elements)->next;
    
    content = (linked_element_t *)(*linked_elements)->content;
    content->caps = caps;
}

void                    semantic_block_caps(list_t **linked_elements, ast_node_t *node) {

    char                *media_type = AST_GET_VALUE(node, "media_type");

    if (media_type == NULL) {
        g_printerr(SEMANTIC_ERROR_MT_CAPS_O);
        return ;
    }

    if (strncmp(media_type, "video", 5) == 0) {
        semantic_block_caps_video(linked_elements, node);
    } else if (strncmp(media_type, "audio", 5) == 0) {
        semantic_block_caps_audio(linked_elements, node);
    }
}
