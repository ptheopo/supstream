#ifndef SEMANTIC_H
# define SEMANTIC_H

# include <ctype.h>
# include <fcntl.h>
# include <glib.h>
# include <gst/gst.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>

# include "ast.h"
# include "config.h"
# include "def.h"
# include "linked.h"
# include "supstream.h"
# include "verify.h"

/*
 * Write Gstreamer elements in AST and symtable for a quickly access
 */

/* Functions are used only by semantic part for the moment */

void                    exit_block(ast_node_t **node);

void                    pipeline_block(ast_node_t **node);
void                    properties_block(ast_node_t *node, GstElement *element);
void                    pads_block(
                        list_t **linked_pads,
                        ast_node_t *node,
                        GstElement *element);
void                    linked_element_line(
                        list_t **linked_elements,
                        GstElement *element,
                        char *element_name);
void                    caps_block(
                        list_t **linked_elements,
                        ast_node_t *node);
linked_result_t         *elements_block(
                        ast_node_t **node,
                        GHashTable **symtable,
                        GstPipeline *pipeline);
void                    semantic_apply_config(
                        ast_node_t *node,
                        config_t **config);
void                    semantic_apply_config_pipeline(
                        ast_node_t *node,
                        config_pipeline_t **config_pipeline);

/* Dynamic types for properties */

typedef union           dtype_u {
    guint64             guint64;
    gfloat              gfloat;
    gboolean            gboolean;
    gint                gint;
    guint               guint;
}                       dtype_t;

supstream_t             *semantic_apply(ast_tree_t **root);

#endif
