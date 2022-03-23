#ifndef AST_H
# define AST_H

# include <cjson/cJSON.h>
# include <gst/gst.h>
# include <stdlib.h>
# include <string.h>
# include <glib.h>
# include <pthread.h>

# include "ast.h"
# include "config.h"
# include "list.h"

/* AST structure */

# define iKEY                   (unsigned int) 0
# define iVALUE                 (unsigned int) 1
# define iSCALAR                (unsigned int) 2
# define iBLOCK                 (unsigned int) 3
# define iLINE                  (unsigned int) 4

/* Macros */

# define AST_GET_VALUE(node, key) \
    ast_iscalar_value(ast_iscalar_get_by_key(node, key))
# define AST_IBLOCK_IS(node, block) \
    (ast_iblock_is(node, block) == TRUE)
# define AST_IS_IBLOCK(node) \
    (ast_node_is_iblock(node) == TRUE)
# define AST_RCHILD(node) \
    (node \
     && node->right)
# define AST_KEY_IS(node, key) \
    (node \
     && node->left \
     && node->left->left \
     && strcmp(node->left->left->str, key) == 0)

/*
 * AST root is a YAML "document"
 * iBLOCK and iLINE at left of iBLOCK
 * iBLOCK and iLINE are neighbors from the right
 * iSCALAR at left of iLINE
 * iKEY & iVALUE definition at left and right of iSCALAR
 * deepblock searching parameters by default or specified in function naming (ex: by_key).
 */

typedef enum                    identifier_e {

    IDENT_BLOCK                 = iBLOCK,
    IDENT_LINE                  = iLINE,
    IDENT_SCALAR                = iSCALAR,
    IDENT_KEY                   = iKEY,
    IDENT_VALUE                 = iVALUE

} identifier_t;

/* This structure used for semantic part */

typedef union                   semantic_data_u {
    GstPipeline                 *gstpipeline;
    GstElement                  *gstelement;
}                               semantic_data_t;

typedef struct                  ast_node_s {
    char                        *str;
    identifier_t                identifier;
    pthread_t                   pthread_id; /* NOT EVERYWHERE */
    GHashTable                  *symtable; /* NOT EVERYWHERE */
    semantic_data_t             *sdata; /* NOT EVERYWHERE */
    config_pipeline_t           *config_pipeline; /* NOT EVERYWHERE */
    struct ast_node_s           *right;
    struct ast_node_s           *left;
}                               ast_node_t;
typedef struct ast_node_s       ast_tree_t;

/* General AST functions */

void                            ast_free(ast_tree_t **root);

/* General Node functions */

ast_node_t                      *ast_node_new(
                                char *str,
                                identifier_t ident);
void                            ast_node_print(ast_node_t *node);
void                            ast_node_free(ast_node_t *node);
gboolean                        ast_node_is_iblock(ast_node_t *node);
gboolean                        ast_node_is_iscalar(ast_node_t *node);
gboolean                        ast_node_is_iline(ast_node_t *node);
void                            ast_node_remove_by_key(
                                ast_node_t **node,
                                char *str);
cJSON                           *ast_node_tojson(
                                ast_node_t *node,
                                list_t *blockdeep);

/* Deepblocks functions */

list_t                          *ast_deepblock_create(int nb, ...);
void                            ast_deepblock_free(list_t *deepblock);

/* iBlock functions */

gboolean                        ast_iblock_is(
                                ast_node_t *node,
                                char *str);
ast_node_t                      *ast_iblock_get(
                                ast_tree_t *root,
                                list_t *blockdeep);
ast_node_t                      *ast_iblock_new(char *str);

ast_node_t                      **aast_iblock_get(
                                ast_tree_t **root,
                                list_t *blockdeep);

/* iLine functions */

ast_node_t                      *ast_iline_new(ast_node_t *scalar);

/* iScalar functions */

ast_node_t                      *ast_iscalar_get_by_key(
                                ast_node_t *node,
                                char *key);
ast_node_t                      **aast_iscalar_get_by_key(
                                ast_node_t **node,
                                char *key);
void                            ast_iscalar_set_simple(ast_node_t **node, char *key, char *value);
char                            *ast_iscalar_value(ast_node_t *scalar_node);
ast_node_t                      *ast_iscalar_new(ast_node_t *key, ast_node_t *value);
ast_node_t                      *ast_iscalar_new_simple(char *key, char *value);

/* iLine / iBlock functions */

void                            ast_ilb_add(
                                ast_tree_t **root,
                                ast_node_t *node,
                                list_t *blockdeep);

/* Browse getters - Browsing in AST (prefix, infix, suffix) using callback */

void                            ast_browse_prefix(
                                ast_node_t *node,
                                void (*fn)(ast_node_t *));
void                            ast_browse_infix(
                                ast_node_t *node,
                                void (*fn)(ast_node_t *));
void                            ast_browse_postfix(
                                ast_node_t *node,
                                void (*fn)(ast_node_t *));

/* Browse setters - Browsing & Apply in AST (prefix, infix, suffix) using callback */

void                            aast_browse_prefix(
                                ast_node_t **node,
                                void (*fn)(ast_node_t **));
void                            aast_browse_infix(
                                ast_node_t **node,
                                void (*fn)(ast_node_t **));
void                            aast_browse_postfix(
                                ast_node_t **node,
                                void (*fn)(ast_node_t **));

#endif
