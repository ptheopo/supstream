#ifndef SUPSTREAM_H
# define SUPSTREAM_H

# include "ast.h"
# include "config.h"

/* General Supstream structure, before execution */

typedef struct          supstream_s {
    config_t            *config;
    ast_tree_t          **root;
}                       supstream_t;

#endif
