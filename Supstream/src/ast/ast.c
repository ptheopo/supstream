#include "ast.h"

static void     ast_node_tojson_recurse(ast_node_t *node, cJSON **block) {

    cJSON       *new_block = NULL;
    cJSON       *new_line_value = NULL;
    char        *new_line_key = NULL;

    while (node != NULL) {

        if (ast_node_is_iblock(node)) {

            /* iBlock translation */
            if (*block == NULL) {
                /* block root not initialized */
                *block = cJSON_CreateObject();
                ast_node_tojson_recurse(node->left, block);
                break ;
            } else {
                new_block = cJSON_CreateObject();
                cJSON_AddItemToObject(*block, node->str, new_block);
                ast_node_tojson_recurse(node->left, &new_block);
            }

        } else if (*block != NULL && ast_node_is_iline(node)) {

            /* iLine translation */
            new_line_key = node->left->left->str;
            new_line_value = cJSON_CreateString(node->left->right->str);
            cJSON_AddItemToObject(*block, new_line_key, new_line_value);

        }

        node = node->right;

    }
}

cJSON           *ast_node_tojson(
                ast_node_t *node,
                list_t *blockdeep) {

    ast_node_t  *iblock = ast_iblock_get(node, blockdeep);
    cJSON       *result = NULL;

    ast_node_tojson_recurse(iblock, &result);
    return (result);
}

static void     ast_node_jsonstr_toast_recurse(cJSON *ast_json, ast_node_t **result) {

    cJSON       *node_json = NULL;
    ast_node_t  *scalar = NULL;
    ast_node_t  *block = NULL;
    ast_node_t  *line = NULL;

    if (ast_json == NULL)
        return ;

    cJSON_ArrayForEach(node_json, ast_json)
    {

        if (cJSON_IsObject(node_json) == TRUE) {

            /* iBLOCK */
            block = ast_iblock_new(g_strdup(node_json->string));
            ast_ilb_add_simple(result, block);
            ast_node_jsonstr_toast_recurse(node_json, &(block->left));

        } else {

            /* iSCALAR (iKEY + iVALUE) */
            scalar = ast_iscalar_new_simple(g_strdup(node_json->string), g_strdup(node_json->valuestring));
            line = ast_iline_new(scalar);
            ast_ilb_add_simple(result, line);

        }

    }

}

/* This is not a toast, this function convert JSON string to an Abstract Syntax Tree */

ast_node_t      *ast_node_jsonstr_toast(
                cJSON *json) {

    ast_node_t  *result = NULL;

    ast_node_jsonstr_toast_recurse(json, &result);
    return (result);
}

gboolean        ast_iblock_is(
                ast_node_t *node,
                char *str) {

    if (node != NULL
        && str != NULL
        && node->identifier == iBLOCK
        && strcmp(node->str, str) == 0) {
        return (TRUE);
    }
    return (FALSE);
}

ast_node_t      *ast_iscalar_get_by_key(
                ast_node_t *node,
                char *key) {

    if (node == NULL
        || node->left == NULL)
        return (NULL);

    node = node->left;

    while (node) {

        if (node->identifier == iLINE
            && strcmp(node->left->left->str, key) == 0)
            return (node->left);

        node = node->right;
        
    }
    return (NULL);
}

ast_node_t      **aast_iscalar_get_by_key(
                ast_node_t **node,
                char *key) {

    ast_node_t  *node_ptr = *node;
    ast_node_t  **node_tmp = NULL;

    if (node == NULL
        || *node == NULL
        || (*node)->left == NULL)
        return (NULL);

    *node = (*node)->left;

    while (*node) {

        if ((*node)->identifier == iLINE
            && strcmp((*node)->left->left->str, key) == 0) {
            node_tmp = &((*node)->left);
            *node = node_ptr;
            return (node_tmp);
        }

        *node = (*node)->right;
        
    }
    *node = node_ptr;
    return (NULL);
}

ast_node_t      *ast_iscalar_new_simple(char *key, char *value) {

    ast_node_t  *new_key_node = ast_node_new(key, iKEY);
    ast_node_t  *new_value_node = ast_node_new(value, iVALUE);
    ast_node_t  *new_scalar_node = ast_iscalar_new(new_key_node, new_value_node);

    return (new_scalar_node);
}

/* Considering node variable is the parent block */

void            ast_iscalar_set_simple(ast_node_t **node, char *key, char *value) {

    list_t      *deepblock = NULL;
    ast_node_t  **scalar = aast_iscalar_get_by_key(node, key);
    ast_node_t  *new_scalar = NULL;
    ast_node_t  *new_iline = NULL;

    if (scalar == NULL) {

        /* Create new scalar */
        deepblock = ast_deepblock_create(1, (*node)->str);
        new_scalar = ast_iscalar_new_simple(key, value);
        new_iline = ast_iline_new(new_scalar);
        if (new_scalar == NULL || new_iline == NULL || deepblock == NULL)
            return ;
        ast_ilb_add(node, new_iline, deepblock);

    } else {

        /* Set scalar */
        free((*scalar)->right->str);
        (*scalar)->right->str = g_strdup(value);

    }
}

static void     ast_deepblock_free_del(void *content, size_t size) {

    (void)size;
    free(content);
}

void            ast_deepblock_free(list_t *deepblock) {

    lstdel(&deepblock, ast_deepblock_free_del);
}

list_t          *ast_deepblock_create(int nb, ...) {

    va_list     params;
    char        *av = NULL;
    list_t      *new = NULL;
    list_t      *deepblock = NULL;

    va_start(params, nb);
    while (nb > 0) {
        av = va_arg(params, char *);
        new = lstnew(av, sizeof(char) * (strlen(av) + 1));
        if (new == NULL)
            return (NULL);
        lstaddlast(&deepblock, new);
        nb--;
    }
    va_end(params);
    return (deepblock);
}

void            ast_browse_prefix(
                ast_node_t *node,
                void (*fn)(ast_node_t *)) {

    fn(node);
    if (node->left != NULL)
        ast_browse_prefix(node->left, fn);
    if (node->right != NULL)
        ast_browse_prefix(node->right, fn);
}

void            ast_browse_infix(
                ast_node_t *node,
                void (*fn)(ast_node_t *)) {

    if (node->left != NULL)
        ast_browse_infix(node->left, fn);
    fn(node);
    if (node->right != NULL)
        ast_browse_infix(node->right, fn);
}

void            ast_browse_postfix(
                ast_node_t *node,
                void (*fn)(ast_node_t *)) {

    if (node->left != NULL)
        ast_browse_postfix(node->left, fn);
    if (node->right != NULL)
        ast_browse_postfix(node->right, fn);
    fn(node);
}

void            aast_browse_prefix(
                ast_node_t **node,
                void (*fn)(ast_node_t **)) {

    fn(node);
    if ((*node)->left != NULL)
        aast_browse_prefix(&((*node)->left), fn);
    if ((*node)->right != NULL)
        aast_browse_prefix(&((*node)->right), fn);
}

void            aast_browse_infix(
                ast_node_t **node,
                void (*fn)(ast_node_t **)) {

    if ((*node)->left != NULL)
        aast_browse_infix(&((*node)->left), fn);
    fn(node);
    if ((*node)->right != NULL)
        aast_browse_infix(&((*node)->right), fn);
}

void            aast_browse_postfix(
                ast_node_t **node,
                void (*fn)(ast_node_t **)) {

    if ((*node)->left != NULL)
        aast_browse_postfix(&((*node)->left), fn);
    if ((*node)->right != NULL)
        aast_browse_postfix(&((*node)->right), fn);
    fn(node);
}

gboolean        ast_node_is_iline(ast_node_t *node) {

    if (node != NULL
            && node->left != NULL
            && node->identifier == iLINE
            && node->left->identifier == iSCALAR
            && node->str == NULL)
        return (TRUE);
    return (FALSE);
}

gboolean        ast_node_is_iblock(ast_node_t *node) {

    if (node != NULL
            && node->identifier == iBLOCK
            && node->str != NULL)
        return (TRUE);
    return (FALSE);
}

gboolean        ast_node_is_iscalar(ast_node_t *node) {

    if (node != NULL
            && node->identifier == iSCALAR
            && node->left != NULL
            && node->right != NULL
            && node->left->str != NULL
            && node->right->str != NULL)
        return (TRUE);
    return (FALSE);
}

void            ast_node_print(ast_node_t *node) {

    if (node == NULL)
        return ;

    if (ast_node_is_iscalar(node))
        g_print("%s : %s\n", node->left->str, node->right->str);
    else if (ast_node_is_iblock(node))
        g_print("%s\n", node->str);
}

void            ast_node_free(ast_node_t *node) {

    /* /!\ Warning with free memory from parsing */
    /* /!\ free from gst_bin  (https://gstreamer.freedesktop.org/documentation/gstreamer/gstbin.html?gi-language=c) using gst_bin_remove*/
    if (node->str != NULL)
        free(node->str);
    if (node->sdata != NULL)
        free(node->sdata);
    free(node);
}

void            ast_free(ast_tree_t **root) {

    if (root && *root) {
        ast_browse_postfix(*root, &ast_node_free);
        *root = NULL;
    }
}

void            ast_ilb_add_simple(
                ast_tree_t **root,
                ast_node_t *node) {

    ast_node_t  *tmp = *root;

    if (*root == NULL) {
        *root = node;
        return ;
    }

    while ((*root)->right != NULL)
        *root = (*root)->right;
    (*root)->right = node;

    *root = tmp;
}

void            ast_ilb_add(
                ast_tree_t **root,
                ast_node_t *node,
                list_t *blockdeep) {

    ast_node_t  *tmp = NULL;

    if (blockdeep == NULL) return ;

    if ((char *)blockdeep->content != NULL
        && *root != NULL
        && (*root)->str != NULL
        && strcmp((*root)->str, (char *)blockdeep->content) == 0
        && (*root)->identifier == iBLOCK) {

        blockdeep = blockdeep->next;

        if (blockdeep == NULL) {

            /* if all deep blocks poped, then insert */
            if ((*root)->left == NULL) {

                /* insert first line or block */
                (*root)->left = node;

            } else {

                /* insert after multiples lines or blocks */
                tmp = *root;
                *root = (*root)->left;

                while ((*root)->right != NULL)
                    *root = (*root)->right;
                (*root)->right = node;

                *root = tmp;

            }
            return ;

        }
    }

    /* browsing */
    if (*root && (*root)->left)
        ast_ilb_add(&((*root)->left), node, blockdeep);
    if (*root && (*root)->right)
        ast_ilb_add(&((*root)->right), node, blockdeep);

}

/* Warning: This function is used to get dptr from a tree/node, usefull
for btree updates per examples */

ast_node_t      **aast_iblock_get(
                ast_tree_t **root,
                list_t *blockdeep) {

    ast_node_t  **res = NULL;

    if (blockdeep == NULL) return (NULL);

    if ((char *)blockdeep->content != NULL
        && *root != NULL
        && (*root)->str != NULL
        && strcmp((*root)->str, (char *)blockdeep->content) == 0
        && (*root)->identifier == iBLOCK) {

        blockdeep = blockdeep->next;

        if (blockdeep == NULL) return (root);

    }

    /* browsing */
    if ((*root)->left) {
        res = aast_iblock_get(&((*root)->left), blockdeep);
        if (res != NULL)
            return (res);
    }
    if ((*root)->right) {
        res = aast_iblock_get(&((*root)->right), blockdeep);
        if (res != NULL)
            return (res);
    }

    return (NULL);
}

ast_node_t      *ast_iblock_get(
                ast_tree_t *root,
                list_t *blockdeep) {

    ast_node_t  *res = NULL;

    if (blockdeep == NULL) return (NULL);

    if ((char *)blockdeep->content != NULL
        && root != NULL
        && root->str != NULL
        && strcmp(root->str, (char *)blockdeep->content) == 0
        && root->identifier == iBLOCK) {

        blockdeep = blockdeep->next;

        if (blockdeep == NULL) return (root);

    }

    /* browsing */
    if (root->left) {
        res = ast_iblock_get(root->left, blockdeep);
        if (res != NULL)
            return (res);
    }
    if (root->right) {
        res = ast_iblock_get(root->right, blockdeep);
        if (res != NULL)
            return (res);
    }

    return (NULL);
}

ast_node_t      *ast_iblock_new(char *str) {

    return (ast_node_new(str, iBLOCK));
}

void            ast_iscalar_free(ast_node_t *node) {

    if (!AST_IS_ISCALAR (node))
        return ;

    ast_node_free(node->left);
    ast_node_free(node->right);
    ast_node_free(node);

}

void            ast_iline_free(ast_node_t *node) {

    if (AST_IS_ISCALAR (node->left))
        ast_iscalar_free(node->left);

    ast_node_free(node);

}

static void     ast_iblock_free_cb(ast_node_t *node) {

    if (AST_IS_ISCALAR (node))
        ast_iscalar_free(node);
    else if (AST_IS_ILINE (node))
        ast_iline_free(node);
    else if (AST_IS_IBLOCK (node))
        ast_iblock_free(node);

}

void            ast_iblock_free(ast_node_t *node) {

    ast_browse_postfix(node->left, ast_iblock_free_cb);
    ast_node_free(node);

}

void            ast_iblock_remove_by_key(
                ast_tree_t **root,
                list_t *blockdeep,
                char *key) {

    ast_node_t  **pipeline = aast_iblock_get(root, blockdeep);
    ast_node_t  *ptr = *pipeline;
    ast_node_t  *prev = NULL;
    ast_node_t  *next = NULL;

    /* Switch in iLINE/iBLOCK childs */
    ptr = ptr->left;

    while (ptr) {

        /* iBLOCK to remove */

        if (AST_IBLOCK_IS (ptr, key)) {

            if (prev == NULL) {
                next = ptr->right;
                ast_iblock_free(ptr);
                ptr->left = next;
            } else {
                next = ptr->right;
                ast_iblock_free(ptr);
                prev->right = next;
            }

            return ;
            
        }

        prev = ptr;
        ptr = ptr->right;

    }
}

ast_node_t      *ast_iline_new(ast_node_t *scalar) {

    ast_node_t  *node = ast_node_new(NULL, iLINE);

    if (node == NULL) return (NULL);

    node->left = scalar;
    return (node);
}

ast_node_t      *ast_iscalar_new(
                ast_node_t *key,
                ast_node_t *value) {

    ast_node_t  *node = ast_node_new(NULL, iSCALAR);

    if (node == NULL) return (NULL);

    node->left = key;
    node->right = value;
    return (node);
}

ast_node_t      *ast_node_new(
                char *str,
                identifier_t ident) {

    ast_node_t  *node = NULL;

    node = (ast_node_t *)malloc(sizeof(ast_node_t));
    if (node == NULL) return (NULL);

    node->str           = str;
    node->identifier    = ident;
    node->sdata         = NULL;
    node->left          = NULL;
    node->right         = NULL;
    
    return (node);
}

char            *ast_iscalar_value(ast_node_t *scalar_node) {

    if (scalar_node
            && scalar_node->right
            && scalar_node->right->str)
        return (scalar_node->right->str);
    return (NULL);
}

/* Considering node variable is the parent block */

void            ast_node_remove_by_key(
                ast_node_t **node,
                char *str) {

    ast_node_t *prev = NULL;
    ast_node_t *tmp = *node;
    ast_node_t *parent_block = *node;
    ast_node_t *tmp_right = NULL;

    tmp = tmp->left;
    while (tmp) {
        if (ast_node_is_iline(tmp) == TRUE
                && AST_KEY_IS (tmp, str)) {
            if (prev == NULL) {
                tmp_right = tmp->right;
                ast_node_free(tmp);
                parent_block->left = tmp_right;
            } else {
                tmp_right = tmp->right;
                ast_node_free(tmp);
                prev->right = tmp_right;
            }
        }
        prev = tmp;
        tmp = tmp->right;
    }
}
