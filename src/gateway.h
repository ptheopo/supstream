#ifndef GATEWAY_H
# define GATEWAY_H

# include <assert.h>
# include <cjson/cJSON.h>
# include <signal.h>
# include <unistd.h>
# include <uuid.h>
# include <zmq.h>

# include "ast.h"
# include "def.h"
# include "config.h"
# include "request.h"

void                *gateway(void *privdata);

/* Usefull privdata */

typedef struct      data_s {
    ast_tree_t      **root;
    config_t        *config;
    char            *uuid;
}                   data_t;

/* Param */

typedef struct      gatewaydata_s {
    config_t        *config;
    ast_tree_t      **root;
}                   gatewaydata_t;

#endif
