#ifndef SEMANTIC_H
# define SEMANTIC_H
# define _XOPEN_SOURCE

# include <ctype.h>
# include <fcntl.h>
# include <glib.h>
# include <gst/gst.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <time.h>
# include <unistd.h>

# include "ast.h"
# include "config.h"
# include "def.h"
# include "link.h"
# include "pev.h"
# include "supstream.h"
# include "verify.h"

/*
 * Write Gstreamer elements in AST and symtable for a quickly access
 */

/* Functions are used only by semantic part for the moment */

void                    exit_block(ast_node_t **node);

void                    semantic_block_pipelines(ast_node_t **node);
void                    semantic_block_properties(ast_node_t *node, GstElement *element);
void                    semantic_block_pads(
                        list_t **linked_pads,
                        ast_node_t *node,
                        GstElement *element);
void                    semantic_block_pad_props(
                        list_t **pad_props,
                        ast_node_t *node,
                        GstElement *element);
void                    semantic_line_linked_element(
                        list_t **linked_elements,
                        GstElement *element,
                        char *element_name);

void                    semantic_line_start_playing(
                        GstElement *element,
                        char *delay);
void                    semantic_line_start_paused(
                        GstElement *element,
                        char *delay);
void                    semantic_line_start_null(
                        GstElement *element,
                        char *delay);
void                    semantic_line_start_ready(
                        GstElement *element,
                        char *delay);

void                    semantic_block_caps(
                        list_t **linked_elements,
                        ast_node_t *node);
linked_result_t         *semantic_block_elements(
                        ast_node_t **node,
                        GHashTable **symtable,
                        GstPipeline *pipeline);
void                    semantic_block_element(
                        GHashTable **symtable,
                        GstPipeline *pipeline,
                        ast_node_t **node,
                        list_t **linked_elements,
                        list_t **linked_pads,
                        list_t **pad_props_lst);
void                    semantic_config(
                        ast_node_t *node,
                        config_t **config);
void                    semantic_config_pipeline(
                        ast_node_t *node,
                        config_pipeline_t **config_pipeline);

void                    semantic_link(
                        GHashTable **symtable,
                        list_t **linked_elements,
                        list_t **linked_pads,
                        list_t **linked_pad_props);

supstream_t             *semantic_doc(ast_tree_t **root);

/* Dynamic types for properties */

typedef union           dtype_u {
    guint64             guint64;
    gfloat              gfloat;
    gboolean            gboolean;
    gint                gint;
    guint               guint;
    GstCaps             *gstcaps;
    const GValue        *gvalue;
}                       dtype_t;

#endif
