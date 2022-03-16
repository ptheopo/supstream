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
