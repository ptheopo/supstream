#ifndef REQUEST_H
# define REQUEST_H

# define NB_REQUEST 15

# include <cjson/cJSON.h>
# include <math.h>
# include <stdio.h>

# include "ast.h"
# include "def.h"
# include "semantic.h"

# define STATE_IS_NULL(s) \
    (strcmp(s, "null") == 0)
# define STATE_IS_READY(s) \
    (strcmp(s, "ready") == 0)
# define STATE_IS_PAUSE(s) \
    (strcmp(s, "pause") == 0)
# define STATE_IS_PLAY(s) \
    (strcmp(s, "play") == 0)

/*
 * All requests are launched from supstream-gateway using Redis pub/sub in JSON
 */

typedef struct  request_s {
    char        *name;
    char        *(*fn)(const cJSON *, ast_tree_t **);
}               request_t;

/*
 * These functions are usefull for request parsing or others
 */

GstElement      *request_get_element(gchar *element, ast_node_t *root);

/*
 * These functions can be called in an other update context
 */

int             state_update(ast_node_t **pipeline, char *state_str);

/*
 * Show
 * ====
 *
 * Used to get general pipelines configuration, saved in cache 
 *
 * {
 *   command: "show",
 * }
 *
 */

char            *request_show(
                const cJSON *request_json,
                ast_tree_t **root);

/*
 * Show pipeline
 * =============
 *
 * Used to get a specific pipeline, saved in cache 
 *
 * {
 *   command: "show",
 *   pipeline: "matroska_video_0"
 * }
 *
 */

char            *request_show_pipeline(
                const cJSON *request_json,
                ast_tree_t **root);

/*
 * Unlink element
 * ==============
 *
 * {
 *   command: "unlink_element",
 *   pipeline: "matroska-video_0",
 *   src: "videoconvert",
 *   sink: "autovideosink"
 * }
 *
 */

char            *request_unlink_elements(
                const cJSON *request_json,
                ast_tree_t **root);

/*
 * Link element
 * ============
 *
 * {
 *   command: "link_element",
 *   pipeline: "matroska-video_0",
 *   src: "videoconvert",
 *   sink: "autovideosink"
 * }
 *
 */

char            *request_link_elements(
                const cJSON *request_json,
                ast_tree_t **root);

/*
 * Create element
 * ==============
 *
 * Create a new element, instantly available.
 *
 * {
 *   command: "create_element",
 *   pipeline: "matroska-video_0",
 *   element: {
 *      ... same from yaml configuration but in JSON ...
 *   }
 * }
 *
 */

char            *request_create_element(
                const cJSON *request_json,
                ast_tree_t **root);

/*
 * Remove element
 * ==============
 *
 * Don't forget to unlink your element before using this command ;)
 *
 * {
 *   command: "remove_element",
 *   pipeline: "matroska-video_0",
 *   element: "autovideosink"
 * }
 *
 */

char            *request_remove_element(
                const cJSON *request_json,
                ast_tree_t **root);

/*
 * Set properties
 * ==============
 *
 * Modify properties of a given element in a specific pipeline
 *
 * {
 *   command: "set_properties",
 *   pipeline: "matroska-video_0",
 *   element: "videoconvert",
 *   properties: {
 *     ...
 *   }
 * }
 *
 */

char            *request_set_properties(
                const cJSON *request_json,
                ast_tree_t **root);

/*
 * Set pad properties
 * ==================
 *
 * Modify pad properties of a given element in a specific pipeline
 *
 * {
 *   command: "set_properties",
 *   pipeline: "matroska-video_0",
 *   element: "videoconvert",
 *   pad: "video_0",
 *   properties: {
 *     ...
 *   }
 * }
 *
 */

char            *request_set_pad_properties(
                const cJSON *request_json,
                ast_tree_t **root);


/*
 * Set caps
 * ========
 *
 * Modify caps of a given element in a specific pipeline
 *
 * {
 *   command: "set_caps",
 *   pipeline: "matroska-video_0",
 *   element: "videoconvert",
 *   caps: {
 *     ...
 *   }
 * }
 *
 */

char            *request_set_caps(
                const cJSON *request_json,
                ast_tree_t **root);

/*
 * Version
 * =======
 *
 * {
 *   command: "version"
 * }
 *
 */

char            *request_version(
                const cJSON *request_json,
                ast_tree_t **root);

/*
 * Pause
 * =====
 *
 * Update state to PAUSED for a specific pipeline or rather all
 *
 * {
 *   command: "pause"
 *   pipelines: ["matroska-video_0"]
 * }
 *
 */

char            *request_pause(
                const cJSON *request_json,
                ast_tree_t **root);

/*
 * Play
 * ====
 *
 * Update state to PLAYING for a specific pipeline or rather all
 *
 * {
 *   command: "play"
 *   pipelines: ["matroska-video_0"]
 * }
 *
 */

char            *request_play(
                const cJSON *request_json,
                ast_tree_t **root);

/*
 * Ready
 * =====
 *
 * Update state to READY for a specific pipeline or rather all
 *
 * {
 *   command: "ready"
 *   pipelines: ["matroska-video_0"]
 * }
 *
 */

char            *request_ready(
                const cJSON *request_json,
                ast_tree_t **root);

/*
 * Null
 * ====
 *
 * Update state to NULL for a specific pipeline or rather all
 *
 * {
 *   command: "null"
 *   pipelines: ["matroska-video_0"]
 * }
 *
 */

char            *request_null(
                const cJSON *request_json,
                ast_tree_t **root);

/*
 * Restart
 * =======
 *
 * This request restart all pipelines
 *
 * {
 *   command: "restart"
 * }
 *
 */

char            *request_restart(
                const cJSON *request_json,
                ast_tree_t **root);

/*
 * Exit
 * ====
 *
 * This request exit a supstream process
 *
 * {
 *   command: "exit"
 * }
 *
 */

char            *request_exit(
                const cJSON *request_json,
                ast_tree_t **root);

/*
 *
 * Procedures
 * ==========
 *
 * Look at associated sample configuration,
 * located at samples/matroska_video_0_pre_defined.yaml
 *
 * 1. {
 *   command: "create_procedure",
 *   procedure: {
 *     enable_shagadelic_effect: [
 *       {
 *         command: "state",
 *         pipeline: "matroska-video_0",
 *         state: "pause"
 *       },
 *       {
 *         command: "unlink",
 *         pipeline: "matroska-video_0",
 *         src: "vp8dec",
 *         sink: "videoconvert"
 *       },
 *       {
 *         command: "unlink",
 *         pipeline: "matroska-video_0",
 *         src: "videoconvert",
 *         sink: "autovideosink"
 *       },
 *       {
 *         command: "link",
 *         pipeline: "matroska-video_0",
 *         src: "vp8dec",
 *         sink: "video_convert"
 *       },
 *       {
 *         command: "link",
 *         pipeline: "matroska-video_0",
 *         src: "video_convert",
 *         sink: "effect_shagadelictv"
 *       },
 *       {
 *         command: "link",
 *         pipeline: "matroska-video_0",
 *         src: "effect_shagadelictv",
 *         sink: "effect_videoconvert2"
 *       },
 *       {
 *         command: "link",
 *         pipeline: "matroska-video_0",
 *         src: "effect_videoconvert2",
 *         sink: "autovideosink"
 *       },
 *       {
 *         command: "state",
 *         pipeline: "matroska-video_0",
 *         state: "play"
 *       }
 *     ]
 *   }
 * }
 *
 * 2. {
 *   command: "create_procedure",
 *   procedure: {
 *     disable_shagadelic_effect: [
 *       {
 *         command: "state",
 *         pipeline: "matroska-video_0",
 *         state: "pause"
 *       },
 *       {
 *         command: "unlink",
 *         pipeline: "matroska-video_0",
 *         src: "vp8dec",
 *         sink: "videoconvert"
 *       },
 *       {
 *         command: "unlink",
 *         pipeline: "matroska-video_0",
 *         src: "videoconvert",
 *         sink: "effect_shagadelictv"
 *       },
 *       {
 *         command: "unlink",
 *         pipeline: "matroska-video_0",
 *         src: "effect_shagadelictv",
 *         sink: "effect_videoconvert2"
 *       },
 *       {
 *         command: "unlink",
 *         pipeline: "matroska-video_0",
 *         src: "effect_videoconvert2",
 *         sink: "autovideosink"
 *       },
 *       {
 *         command: "link",
 *         pipeline: "matroska-video_0",
 *         src: "vp8dec",
 *         sink: "videoconvert"
 *       },
 *       {
 *         command: "link",
 *         pipeline: "matroska-video_0",
 *         src: "videoconvert",
 *         sink: "autovideosink"
 *       },
 *       {
 *         command: "state",
 *         pipeline: "matroska-video_0",
 *         state: "play"
 *       }
 *     ]
 *   }
 * }
 *
 * 3. {
 *   command: "call_procedure",
 *   name: "enable_shagadelic_effect"
 * }
 *
 * What's the benefice to use procedural syntax ?
 *
 * - Better historic support
 * - Re-use multiple command easily
 * - Mix multiple procedures
 *
 */

/* (For the future) */

#endif
