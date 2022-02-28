#include "config.h"

/* Default supstream */

config_t                *config_default_new(void) {

    config_t            *config = (config_t *)malloc(sizeof(config_t));
    char                cwd[PATH_MAX] = {0};

    if (config == NULL)
        return (NULL);

    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return (NULL);

    /* Default configuration */
    config->working_dir             = g_strdup(cwd);
    config->stdout_logfile          = NULL;
    config->stderr_logfile          = NULL;
    config->bin_to_dotfile_enabled  = FALSE;
    config->zmq_path                = g_strdup(ZMQ_PATH);
    config->env                     = NULL;
    return (config);
}

/* Default pipeline */

config_pipeline_t       *config_pipeline_default_new(void) {

    config_pipeline_t   *config_pipeline = (config_pipeline_t *)malloc(sizeof(config_pipeline_t));
    char                cwd[PATH_MAX] = {0};

    if (config_pipeline == NULL)
        return (NULL);

    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return (NULL);

    /* Default configuration */
    config_pipeline->init_state     = g_strdup("play");
    config_pipeline->directory      = g_strdup(cwd);
    config_pipeline->type_exec      = g_strdup("sync");
    config_pipeline->priority       = CONFIG_PIPELINE_UNDEFINED_PRIORITY;
    config_pipeline->start_secs     = 0;
    config_pipeline->auto_restart   = FALSE;
    config_pipeline->start_datetime = NULL;
    return (config_pipeline);
}

/* Debug */

void                    config_show(config_t *config) {

    g_print("working_dir: %s\n", config->working_dir);
    if (config->stdout_logfile != NULL)
        g_print("stdout_logfile: %s\n", config->stdout_logfile);
    if (config->stderr_logfile != NULL)
        g_print("stderr_logfile: %s\n", config->stderr_logfile);
    g_print("zmq_path: %s\n", config->zmq_path);
    if (config->bin_to_dotfile_enabled == TRUE)
        g_print("bin_to_dotfile_dir: True\n");
    else if (config->bin_to_dotfile_enabled == FALSE)
        g_print("bin_to_dotfile_dir: False\n");
}

void                    config_pipeline_show(config_pipeline_t *config_pipeline) {

    g_print("init_state: %s\n", config_pipeline->init_state);
    g_print("directory: %s\n", config_pipeline->directory);
    g_print("type_exec: %s\n", config_pipeline->type_exec);
    g_print("priority: %d\n", config_pipeline->priority);
    g_print("start_secs: %d\n", config_pipeline->start_secs);
    if (config_pipeline->auto_restart == TRUE)
        g_print("auto_restart: True\n");
    else if (config_pipeline->auto_restart == FALSE)
        g_print("auto_restart: False\n");
}
