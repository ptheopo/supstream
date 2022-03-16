#ifndef PIPELINE_H
# define PIPELINE_H

# include <dlfcn.h>
# include <gst/gst.h>
# include <pthread.h>

# include "ast.h"
# include "bus.h"
# include "config.h"
# include "def.h"
# include "gateway.h"
# include "supstream.h"
# include "verify.h"

typedef struct  execdata_s {
    GstPipeline *pipeline;
    config_t    *config;
    config_pipeline_t    *config_pipeline;
}               execdata_t;

typedef struct  privdata_sync_s {
    ast_tree_t  **root;
    execdata_t  *execdata;
    gboolean    (*fn)(gchar *);
}               privdata_sync_t;

void            exec_pipeline(supstream_t *supstream);

#endif
