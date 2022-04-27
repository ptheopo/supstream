#ifndef PARSE_H
# define PARSE_H

# include <glib.h>
# include <string.h>
# include <yaml.h>

# include "ast.h"
# include "fifo.h"
# include "pipeline.h"
# include "semantic.h"

gboolean    parse_token(
            yaml_token_t *token,
            ast_tree_t **root,
            fifo_t **lines,
            list_t **deepblock);

#endif
