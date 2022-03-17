#ifndef CONFIG_H
# define CONFIG_H

# include "def.h"
# include <glib.h>
# include <unistd.h>

typedef struct      config_s {
    char            *working_dir;
    char            *stdout_logfile;
    char            *stderr_logfile;
    gboolean        zmq_disabled;
    char            *zmq_path;
    gboolean        bin_to_dotfile_enabled;
    char            **env;
}                   config_t;

typedef struct      config_pipeline_s {
    char            *init_state;
    char            *directory;
    char            *type_exec;
    int             priority;
    int             start_secs;
    gboolean        auto_restart;
    char            *start_datetime;
}                   config_pipeline_t;

/* Default */

config_t            *config_new_default(void);
config_pipeline_t   *config_new_default_pipeline(void);

/* Debug */

void                config_show(config_t *config);
void                config_show_pipeline(config_pipeline_t *config_pipeline);

#endif
