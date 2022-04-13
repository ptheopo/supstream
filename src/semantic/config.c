#include "semantic.h"

static int                      semantic_isdir(char *path) {

    struct stat                 buf_stat;
    int                         ret;

    ret = stat(path, &buf_stat);
    if (ret == -1) return (0);
    if (!S_ISDIR (buf_stat.st_mode)) return (0);
    return (1);
}

static void                     semantic_config_log(config_t *config) {

    int                         fd;
    char                        *stdout_logfile = config->stdout_logfile;
    char                        *stderr_logfile = config->stderr_logfile;

    /* Redirect stdout to stdout logfile */
    if (config->stdout_logfile != NULL) {
        fd = open(stdout_logfile, O_WRONLY | O_APPEND | O_CREAT);
        if (fd == -1)
            return ;
        dup2(fd, STDOUT_FILENO);
    }

    /* Redirect stderr to stderr logfile */
    if (config->stderr_logfile != NULL) {
        fd = open(stderr_logfile, O_WRONLY | O_APPEND | O_CREAT);
        if (fd == -1)
            return ;
        dup2(fd, STDERR_FILENO);
    }
}

void                            semantic_config(
                                ast_node_t *node,
                                config_t **config) {

    char                        *_working_dir = AST_GET_VALUE (node, "working_dir");
    char                        *_stdout_logfile = AST_GET_VALUE (node, "stdout_logfile");
    char                        *_stderr_logfile = AST_GET_VALUE (node, "stderr_logfile");
    char                        *_zmq_disabled = AST_GET_VALUE (node, "zmq_disabled");
    char                        *_zmq_path = AST_GET_VALUE (node, "zmq_path");
    char                        *_bin_to_dotfile_enabled = AST_GET_VALUE (
                                node,
                                "bin_to_dotfile_enabled");
    char                        **_env = NULL;
    char                        *_timezone = AST_GET_VALUE (node, "timezone");

    if (config == NULL || *config == NULL)
        return ;

    /* Logging redirections (stdout, stderr) */
    if (_stdout_logfile != NULL) {

        free((*config)->stdout_logfile);
        /* Good dir ? */
        (*config)->stdout_logfile = g_strdup(_stdout_logfile);

    } if (_stderr_logfile != NULL) {

        free((*config)->stderr_logfile);
        /* Good dir ? */
        (*config)->stderr_logfile = g_strdup(_stderr_logfile);

    }

    semantic_config_log(*config);

    /* User configuration (Worst if-forest, need to be review) */
    if (_working_dir != NULL) {

        if (semantic_isdir(_working_dir) != 0) {

            free((*config)->working_dir);
            (*config)->working_dir = g_strdup(_working_dir);

            if (chdir((*config)->working_dir) == 0)
                g_print(DIRECTORY_CWD_O, (*config)->working_dir);

        } else {

            g_printerr(DIRECTORY_CWD_NODIR_ERROR_O, _working_dir);

        }

    } if (_zmq_disabled != NULL) {

        (*config)->zmq_disabled = strtogboolean(_zmq_disabled);

    } if (_zmq_path != NULL) {

        free((*config)->zmq_path);
        /* ZMQ Path already used ? */
        (*config)->zmq_path = g_strdup(_zmq_path);

    } if (_bin_to_dotfile_enabled != NULL) {

        (*config)->bin_to_dotfile_enabled = strtogboolean(_bin_to_dotfile_enabled);
        g_print(DIRECTORY_DOTFILE_O);

    } if (_env != NULL) {

        free((*config)->env);
        (*config)->env = NULL;

    } if (_timezone != NULL) {

        free((*config)->timezone);
        (*config)->timezone = g_strdup(_timezone);
        
    }

}

void                            semantic_config_set_timezone(char *timezone) {

    char                        *tz_env = NULL;

    /* Need to verify Timezone */
    tz_env = g_strjoin("=", "TZ", timezone, NULL);
    if (tz_env == NULL)
        return ;

    /* Update timezone */
    putenv(tz_env);
    tzset();
}

void                            semantic_config_set_delay(config_pipeline_t *config_pipeline, GstPipeline *pipeline) {


    struct                      tm tm;
    time_t                      delay;
    time_t                      current;
    double                      diff;
    char                        *ret = 0;

    if (config_pipeline->set_delay == NULL)
        return ;

    /* Get set_delay and current timestamp */
    ret = strptime(config_pipeline->set_delay, "%Y-%m-%d %H:%M:%S", &tm);
    if (ret == NULL) {
        g_printerr(SEMANTIC_ERROR_DATETIME_FORMAT_O);
        return ;
    }
    delay = mktime(&tm);
    current = time(NULL);

    diff = difftime(delay, current);
    if (diff >= 0) {

        /* Set delay */
        gst_pipeline_set_delay(pipeline, difftime(delay, current) * GST_SECOND);
        g_print(SEMANTIC_DATETIME_O, GST_ELEMENT_NAME(pipeline), config_pipeline->set_delay);

    } else
        g_printerr(SEMANTIC_ERROR_DATETIME_O);
}

void                            semantic_config_pipeline(
                                ast_node_t *node,
                                config_pipeline_t **config_pipeline) {

    char                        *_init_state = AST_GET_VALUE (node, "init_state");
    char                        *_directory = AST_GET_VALUE (node, "directory");
    char                        *_type_exec = AST_GET_VALUE (node, "type_exec");
    char                        *_priority = AST_GET_VALUE (node, "priority");
    char                        *_start_secs = AST_GET_VALUE (node, "start_secs");
    char                        *_auto_restart = AST_GET_VALUE (node, "auto_restart");
    char                        *_start_datetime = AST_GET_VALUE (node, "start_datetime");
    char                        *_set_delay = AST_GET_VALUE (node, "set_delay");

    if (config_pipeline == NULL || *config_pipeline == NULL)
        return ;

    /* Logging redirections (stdout, stderr) */
    if (_priority != NULL) {

        (*config_pipeline)->priority = atoi(_priority);

    } if (_directory != NULL) {

        if (semantic_isdir(_directory) != 0) {
            free((*config_pipeline)->directory);
            (*config_pipeline)->directory = g_strdup(_directory);
        } else {
            g_print(DIRECTORY_CWD_O, _directory);
        }

    } if (_init_state != NULL) {

        if (state_is_play(_init_state) == TRUE || state_is_pause(_init_state) == TRUE) {
            free((*config_pipeline)->init_state);
            (*config_pipeline)->init_state = g_strdup(_init_state);
        }

    } if (_type_exec != NULL) {

        if (run_is_await(_type_exec) == TRUE || run_is_thread(_type_exec) == TRUE) {
            free((*config_pipeline)->type_exec);
            (*config_pipeline)->type_exec = g_strdup(_type_exec);
        }

    } if (_start_secs != NULL) {

        (*config_pipeline)->start_secs = atoi(_start_secs);

    } if (_auto_restart != NULL) {

        (*config_pipeline)->auto_restart = strtogboolean(_auto_restart);

    } if (_start_datetime != NULL) {

        free((*config_pipeline)->start_datetime);
        (*config_pipeline)->start_datetime = g_strdup(_start_datetime);

    } if (_set_delay != NULL) {

        (*config_pipeline)->set_delay = g_strdup(_set_delay);

    }
}
