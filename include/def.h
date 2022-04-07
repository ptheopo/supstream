#ifndef DEF_H
# define DEF_H

/*
 * All Supstream log definitions
 */

/* Info */

# define    SUPSTREAM_VERSION_STR                       "Supstream v0.0.2 - tpoac <t.poac@cyim.com>"

/* Init */

# define    INIT_GSTREAMER_O                            "\033[1;36mINITIALIZE\033[0m\tInitialize Gstreamer\n"
# define    INIT_OPEN_YAML_O                            "\033[1;36mINITIALIZE\033[0m\tReading %s YAML pipeline\n"

/* Semantic */

# define    SEMANTIC_NEW_PIPELINE_O                     "\033[1;32mCREATE\033[0m\t\tPipeline %s created\n"
# define    SEMANTIC_NEW_ELEMENT_O                      "\033[1;32mNEW\033[0m\t\tElement %s named %s created\n"
# define    SEMANTIC_ERROR_PLUGIN_O                     "\033[1;31mERROR\033[0m\t\tPlugin %s not found\033[0m\n"
# define    SEMANTIC_SET_PROPERTY_O                     "\033[1;32mPROPERTY\033[0m\tProperty %s setting to %s\n"
# define    SEMANTIC_ADD_BIN_O                          "\033[1;32mBIN\033[0m\t\tAdd %s element in %s pipeline\n"
# define    SEMANTIC_LINKED_ELEMENT_O                   "\033[1;32mLINK\033[0m\t\tElement %s linked to %s element\n"
# define    SEMANTIC_LINKED_PADS_ALWAYS_O               "\033[1;32mLINK\033[0m\t\tAlways pad %s linked to %s pad\n"
# define    SEMANTIC_LINKED_PADS_ALWAYS_REQUEST_O       "\033[1;32mLINK\033[0m\t\tAlways pad %s linked to %s request pad\n"
# define    SEMANTIC_LINKED_PADS_REQUEST_O              "\033[1;32mLINK\033[0m\t\tRequest pad %s linked to %s pad\n"
# define    SEMANTIC_LINKED_PADS_SOMETIMES_O            "\033[1;32mLINK\033[0m\t\tSometimes pad %s linked to %s pad\n"
# define    SEMANTIC_ERROR_LINKED_PADS_O                "\033[1;31mERROR\033[0m\t\tPad %s not linked to %s\033[0m\n"
# define    SEMANTIC_ERROR_LINKED_PADS_REQUEST_TODO_O   "\033[1;31mERROR\033[0m\t\tRequest pads linking need to be implement !\033[0m\n"
# define    SEMANTIC_ERROR_LINKED_ELEMENT_O             "\033[1;31mERROR\033[0m\t\tElement %s not linked to %s\033[0m\n"
# define    SEMANTIC_ERROR_MT_CAPS_O                    "\033[1;31mERROR\033[0m\t\tCaps need to specify media type\033[0m\n"
//# define    SEMANTIC_ERROR_TIMEZONE_O                   "\033[1;31mERROR\033[0m\t\tPlease define your timezone from TZ in your environment, by default the timezone is Europe/Paris\033[0m\n"
# define    SEMANTIC_ERROR_DATETIME_O                   "\033[1;31mERROR\033[0m\t\tWrong datetime, negative timestamp or wrong format (ex: 2022-04-07 22:00:00)\033[0m\n"
# define    SEMANTIC_ERROR_DATETIME_FORMAT_O            "\033[1;31mERROR\033[0m\t\tWrong datetime format (ex: 2022-04-07 22:00:00)\033[0m\n"
# define    SEMANTIC_DATETIME_O                         "\033[1;32mDATETIME\033[0m\t\tStart %s pipeline at %s\n"

/* Pipeline */

# define    PIPELINE_RUN_THREAD_O                       "\033[1;36mRUN\033[0m\t\tRun %s pipeline in thread mode\033[0m\n"
# define    PIPELINE_ERROR_RUN_THREAD_O                 "\033[1;31mERROR\033[0m\t\tStart %s pipeline in thread mode failed\033[0m\n"
# define    PIPELINE_RUN_AWAIT_O                        "\033[1;36mRUN\033[0m\t\tRun %s pipeline in await mode\033[0m\n"

/* ZeroMQ */

# define    ZMQ_PATH                                    "ipc:///tmp/supstream.pipe"
# define    ZMQ_PATH_SUCCESS_O                          "\033[1;32mGATEWAY\033[0m\t\tZeroMQ gateway successfully bind on %s path \033[0m\n"
# define    ZMQ_BIND_ERROR_O                            "\033[1;31mERROR\033[0m\t\tZeroMQ gateway bind error, try with an other zmq_path value\033[0m\n"

/* Request */

# define    REQUEST_BAD_FORMAT_O                        "\033[1;31mERROR\033[0m\t\tBad type in JSON request\033[0m\n"
# define    REQUEST_PAUSE_SUCCESS_O                     "\033[1;32mREQUEST\033[0m\t\tUpdate to PAUSE state pipelines :\033[0m\n"
# define    REQUEST_PLAY_SUCCESS_O                      "\033[1;32mREQUEST\033[0m\t\tUpdate to PLAY state pipelines :\033[0m\n"
# define    REQUEST_READY_SUCCESS_O                     "\033[1;32mREQUEST\033[0m\t\tUpdate to READY state pipelines :\033[0m\n"
# define    REQUEST_NULL_SUCCESS_O                      "\033[1;32mREQUEST\033[0m\t\tUpdate to NULL state pipelines :\033[0m\n"

# define    REQUEST_UNLINK_SUCCESS_O                    "\033[1;32mREQUEST\033[0m\t\tUnlink element %s on %s\n[0m\n"
# define    REQUEST_LINK_SUCCESS_O                      "\033[1;32mREQUEST\033[0m\t\tLink element %s on %s\n[0m\n"

# define    REQUEST_RESTART_SUCCESS_O                   "\033[1;32mREQUEST\033[0m\t\tRestarting pipelines :\033[0m\n"
# define    REQUEST_SET_PROP_SUCCESS_O                  "\033[1;32mREQUEST\033[0m\t\t%s property set\033[0m\n"
# define    REQUEST_GUESS_SUCCESS_O                     "\t\t- %s\033[0m\n"
# define    REQUEST_GUESS_WARNING_O                     "\033[1;33m\t\t- %s (%s)\033[0m\n"
# define    REQUEST_EXIT_O                              "\033[1;36mINITIALIZE\033[0m\tExit Supstream\n"

/* Directory */

# define    DIRECTORY_CWD_NODIR_ERROR_O                 "\033[1;31mERROR\033[0m\t\t%s working directory unfound\033[0m\n"
# define    DIRECTORY_CWD_O                             "\033[1;36mDIRECTORY\033[0m\tCurrent working directory is %s\n"
# define    DIRECTORY_DOTFILE_O                         "\033[1;36mDIRECTORY\033[0m\tDotfile enabled\n"

/* Config */

# define    CONFIG_PIPELINE_UNDEFINED_PRIORITY -1

/* Signal */

# define    SIGNAL_SIGINT_O                             "\033[1;31mSIGINT\033[0m\t\tAre you sure you want to leave Supstream ? [y/N]\033[0m\n"

#endif
