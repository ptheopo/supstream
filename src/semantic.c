#include "semantic.h"

static void             exit_block(ast_node_t **node) {

    g_printerr(SEMANTIC_ERROR_PLUGIN_O, (*node)->str);
    ast_free(node);
    exit(1);
}

/* pipeline block */

static void             pipeline_block(ast_node_t **node) {

    char                *pipeline_name = (*node)->str;
    GstPipeline         *pipeline = NULL;

    g_print(SEMANTIC_NEW_PIPELINE_O, pipeline_name);
    /* create new pipeline */
    pipeline = (GstPipeline *)gst_pipeline_new(pipeline_name);
    if (pipeline == NULL)
        exit_block(node);
    /* alloc sdata union */
    (*node)->sdata = (semantic_data_t *)malloc(sizeof(semantic_data_t));
    if ((*node)->sdata == NULL)
        exit_block(node);
    (*node)->sdata->gstpipeline = pipeline;
}

/* linked_element line */

static void             linked_element_line(
                        list_t **linked_elements,
                        GstElement *element,
                        char *element_name) {

    linked_element_t    *linked_element = (linked_element_t *)malloc(sizeof(linked_element_t));
    list_t              *new = NULL;

    linked_element->src = element;
    linked_element->caps = NULL;
    linked_element->dst_name = element_name;
    new = lstnew(linked_element, sizeof(linked_element_t));
    if (new == NULL)
        return ;
    lstaddlast(linked_elements, new);
}

static void             properties_block(ast_node_t *node, GstElement *element) {

    GObjectClass        *objClass = G_OBJECT_GET_CLASS(element);
    const gchar         *type_name = NULL;
    char                *property = NULL;
    char                *value = NULL;
    char                *endPtr = NULL;
    dtype_t             type;
    GParamSpec          *spec = NULL;

    node = node->left;
    do {
        property = node->left->left->str;
        value = node->left->right->str;
        spec = g_object_class_find_property(objClass, property);
        if (spec != NULL) {
            type_name = g_type_name(spec->value_type);

            if (strcmp(type_name, "gchararray") == 0) {

                /* gchar * */
                g_object_set(G_OBJECT (element), property, (gchar *)value, NULL);

            } else if (strcmp(type_name, "gfloat") == 0) {

                /* gfloat */
                type.gfloat = (gfloat)strtof(value, &endPtr);
                g_object_set(G_OBJECT (element), property, type.gfloat, NULL);

            } else if (strcmp(type_name, "guint64") == 0) {

                /* guint64 */
                type.guint64 = (guint64)strtol(value, &endPtr, 10);
                g_object_set(G_OBJECT (element), property, type.guint64, NULL);

            } else if (strcmp(type_name, "gboolean") == 0) {

                /* gboolean */
                type.gboolean = strtogboolean(value);
                g_object_set(G_OBJECT (element), property, type.gboolean, NULL);

            } else if (strcmp(type_name, "gint") == 0) {

                /* gint */
                type.gint = (gint)atoi(value);
                g_object_set(G_OBJECT (element), property, type.gint, NULL);

            } else if (strcmp(type_name, "guint") == 0) {

                /* guint (maybe need to use stroul) */
                type.guint = (guint)atoi(value);
                g_object_set(G_OBJECT (element), property, type.guint, NULL);

            }

        }
        g_print(SEMANTIC_SET_PROPERTY_O, property, value);
        node = node->right;
    } while (node);
}

/* pads block (save pad linking name in the future..) */

static void             pads_block_one(
                        list_t **linked_pads,
                        ast_node_t *node,
                        GstElement *element) {

    list_t              *deepblock = lstnew("to", sizeof(char) * 3);
    ast_node_t          *scalar_node = ast_iscalar_get_by_key(node, "from");
    ast_node_t          *scalar_name = NULL;
    ast_node_t          *scalar_pad = NULL;
    ast_node_t          *to_block = NULL;
    list_t              *linked_pad_lst = NULL;
    linked_pad_t        *linked_pad = (linked_pad_t *)malloc(sizeof(linked_pad_t));

    if (scalar_node == NULL)
        return ;
    to_block = ast_iblock_get(node, deepblock);
    scalar_name = ast_iscalar_get_by_key(to_block, "name");
    scalar_pad = ast_iscalar_get_by_key(to_block, "pad");

    linked_pad->src_element = element;
    linked_pad->src_pad_name = scalar_node->right->str;
    linked_pad->dst_element_name = scalar_name->right->str;
    linked_pad->dst_pad_name = scalar_pad->right->str;

    linked_pad_lst = lstnew(linked_pad, sizeof(linked_pad_t));
    lstaddlast(linked_pads, linked_pad_lst);
    ast_deepblock_free(deepblock);
}

static void             pads_block(
                        list_t **linked_pads,
                        ast_node_t *node,
                        GstElement *element) {

    node = node->left;
    while (node) {
        pads_block_one(linked_pads, node, element);
        node = node->right;
    }
}

/* caps block */

static void             caps_block_audio(list_t **linked_elements, ast_node_t *node) {

    linked_element_t    *content = NULL;

    char                *media_type = AST_GET_VALUE (node, "media_type");
    char                *format = AST_GET_VALUE (node, "format");
    char                *channels = AST_GET_VALUE (node, "channels");
    char                *block_align = AST_GET_VALUE (node, "block_align");
    char                *layer = AST_GET_VALUE (node, "layer");
    char                *bitrate = AST_GET_VALUE(node, "bitrate");
    char                *layout = AST_GET_VALUE(node, "layout");

    GstCaps *caps = NULL;

    if (media_type == NULL) {
        g_printerr(SEMANTIC_ERROR_MT_CAPS_O);
        exit(1);
    }

    caps = gst_caps_new_empty_simple(media_type);

    if (format != NULL) {
        gst_caps_set_simple(caps, "format", G_TYPE_STRING, (gchar *)format, NULL);
    } else if (channels != NULL) {
        gst_caps_set_simple(caps, "channels", G_TYPE_INT, (gint)atoi(channels), NULL);
    } else if (layout != NULL) {
        gst_caps_set_simple(caps, "layout", G_TYPE_STRING, (gchar *)layout, NULL);
    } else if (block_align != NULL) {
        gst_caps_set_simple(caps, "block_align", G_TYPE_INT, (gint)atoi(block_align), NULL);
    } else if (layer != NULL) {
        gst_caps_set_simple(caps, "layer", G_TYPE_INT, (gint)atoi(layer), NULL);
    } else if (bitrate != NULL) {
        gst_caps_set_simple(caps, "bitrate", G_TYPE_INT, (gint)atoi(bitrate), NULL);
    }

    if (*linked_elements == NULL)
        return ;

    while ((*linked_elements)->next)
        *linked_elements = (*linked_elements)->next;
    
    content = (linked_element_t *)(*linked_elements)->content;
    content->caps = caps;
}

static void             caps_block_video(list_t **linked_elements, ast_node_t *node) {

    linked_element_t    *content = NULL;

    char                *media_type = AST_GET_VALUE (node, "media_type");
    char                *format = AST_GET_VALUE (node, "format");
    char                *height = AST_GET_VALUE (node, "height");
    char                *framerate = AST_GET_VALUE (node, "framerate");
    char                *max_framerate = AST_GET_VALUE (node, "max_framerate");
    char                *views = AST_GET_VALUE (node, "views");
    char                *interlace_mode = AST_GET_VALUE (node, "interlace_mode");
    char                *chroma_site = AST_GET_VALUE (node, "chroma_site");
    char                *pixel_aspect_ratio = AST_GET_VALUE (node, "pixel_aspect_ratio");
    char                *h263version = AST_GET_VALUE (node, "h263version");
    char                *depth = AST_GET_VALUE (node, "depth");

    GstCaps *caps = NULL;

    if (media_type == NULL) {
        g_printerr(SEMANTIC_ERROR_MT_CAPS_O);
        exit(1);
    }

    caps = gst_caps_new_empty_simple(media_type);

    if (format != NULL) {
        gst_caps_set_simple(caps, "format", G_TYPE_STRING, (gchar *)format, NULL);
    } else if (height != NULL) {
        gst_caps_set_simple(caps, "height", G_TYPE_INT, (gint)atoi(height), NULL);
    } else if (framerate != NULL) {
        //gst_caps_set_simple(caps, "framerate", G_TYPE_FRACTION, (gint)atoi(framerate), NULL);
    } else if (max_framerate != NULL) {
        //gst_caps_set_simple(caps, "max-framerate", G_TYPE_FRACTION, (gint)atoi(max_framerate), NULL);
    } else if (views != NULL) {
        gst_caps_set_simple(caps, "views", G_TYPE_INT, (gint)atoi(views), NULL);
    } else if (interlace_mode != NULL) {
        gst_caps_set_simple(caps, "interlace-mode", G_TYPE_STRING, (gchar *)interlace_mode, NULL);
    } else if (chroma_site != NULL) {
        gst_caps_set_simple(caps, "chroma-site", G_TYPE_STRING, (gchar *)chroma_site, NULL);
    } else if (pixel_aspect_ratio != NULL) {
        //gst_caps_set_simple(caps, "pixel-aspect-ratio", G_TYPE_FRACTION, (gint)atoi(height), NULL);
    } else if (h263version != NULL) {
        gst_caps_set_simple(caps, "h263version", G_TYPE_STRING, (gchar *)h263version, NULL);
    } else if (depth != NULL) {
        gst_caps_set_simple(caps, "depth", G_TYPE_INT, (gint)atoi(depth), NULL);
    }

    if (*linked_elements == NULL)
        return ;

    while ((*linked_elements)->next)
        *linked_elements = (*linked_elements)->next;
    
    content = (linked_element_t *)(*linked_elements)->content;
    content->caps = caps;
}

static void             caps_block(list_t **linked_elements, ast_node_t *node) {

    char                *media_type = AST_GET_VALUE(node, "media_type");

    if (media_type == NULL) {
        g_printerr(SEMANTIC_ERROR_MT_CAPS_O);
        return ;
    }

    if (strncmp(media_type, "video", 5) == 0) {
        caps_block_video(linked_elements, node);
    } else if (strncmp(media_type, "audio", 5) == 0) {
        caps_block_audio(linked_elements, node);
    }
}

/* elements block */

static void             free_props_block(void *content, size_t size) {
    (void)size;
    free(content);
}

static linked_result_t  *elements_block(
        ast_node_t      **node,
        GHashTable      **symtable,
        GstPipeline     *pipeline) {

    ast_node_t          *scalar_node = NULL;
    ast_node_t          *scalar_node_link = NULL;
    ast_node_t          *tmp = *node;
    ast_node_t          *properties = NULL;
    ast_node_t          *pads = NULL;
    ast_node_t          *caps = NULL;
    GstElement          *element = NULL;
    /* specify props block inside all elements */
    list_t              *props_dp = lstnew("properties", sizeof(char) * 11);
    list_t              *pads_dp = lstnew("pad_link", sizeof(char) * 9);
    list_t              *caps_dp = lstnew("caps", sizeof(char) * 5);
    list_t              *linked_elements = NULL;
    list_t              *linked_pads = NULL;
    linked_result_t     *result = (linked_result_t *)malloc(sizeof(linked_result_t));

    if (result == NULL)
        exit(1);
    if ((*node)->left == NULL)
        return (NULL);

    /* move on first element */
    *node = (*node)->left;

    /* for each elements */
    do {

        if (ast_node_is_iblock(*node) == TRUE) {
            /* re-init for each elements */
            scalar_node = NULL;
            scalar_node_link = NULL;
            properties = NULL;
            pads = NULL;
            caps = NULL;
            element = NULL;

            /* obtain element scalar node */
            scalar_node = ast_iscalar_get_by_key(*node, "element");

            /* create new element */
            if (scalar_node == NULL)
                exit_block(node);
            element = (GstElement *)gst_element_factory_make(
                    (const gchar *)scalar_node->right->str,
                    (const gchar *)(*node)->str);
            if (element == NULL)
                exit_block(node);
            g_print(SEMANTIC_NEW_ELEMENT_O, (*node)->str, scalar_node->right->str);

            /* alloc sdata union */
            (*node)->sdata = (semantic_data_t *)malloc(sizeof(semantic_data_t));
            if ((*node)->sdata == NULL)
                exit_block(node);
            (*node)->sdata->gstelement = element;

            /* create properties */
            properties = ast_iblock_get(
                    (*node)->left,
                    props_dp);
            if (properties != NULL)
                properties_block(properties, element);

            /* create bin */
            gst_bin_add(GST_BIN (pipeline), element);
            g_print(SEMANTIC_ADD_BIN_O,
                    scalar_node->right->str,
                    GST_OBJECT_NAME (pipeline));

            /* create linked_element list */
            scalar_node_link = ast_iscalar_get_by_key(*node, "element_link");
            if (scalar_node_link != NULL) {
                linked_element_line(&linked_elements, element, scalar_node_link->right->str);
            } else {
                linked_element_line(&linked_elements, element, NULL);
            }

            /* create caps */
            caps = ast_iblock_get(
                    (*node)->left,
                    caps_dp);
            if (caps != NULL)
                caps_block(&linked_elements, caps);

            /* create linked_pad list */
            pads = ast_iblock_get(
                    (*node)->left,
                    pads_dp);
            if (pads != NULL)
                pads_block(&linked_pads, pads, element);

            /* create element sym in symtable */
            g_hash_table_insert (*symtable, (*node)->str, element);
        }

        *node = (*node)->right;

    } while (*node != NULL);

    lstdel(&props_dp, &free_props_block);
    lstdel(&pads_dp, &free_props_block);
    lstdel(&caps_dp, &free_props_block);
    *node = tmp;
    result->elements = linked_elements;
    result->pads = linked_pads;

    return (result);
}

/* Config */

static int                      semantic_isdir(char *path) {

    struct stat                 buf_stat;
    int                         ret;

    ret = stat(path, &buf_stat);
    if (ret == -1) return (0);
    if (!S_ISDIR (buf_stat.st_mode)) return (0);
    return (1);
}

static void                     semantic_apply_config_log(config_t *config) {

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

static void                     semantic_apply_config(
                                ast_node_t *node,
                                config_t **config) {

    char                        *_working_dir = AST_GET_VALUE (node, "working_dir");
    char                        *_stdout_logfile = AST_GET_VALUE (node, "stdout_logfile");
    char                        *_stderr_logfile = AST_GET_VALUE (node, "stderr_logfile");
    char                        *_zmq_path = AST_GET_VALUE (node, "zmq_path");
    char                        *_bin_to_dotfile_enabled = AST_GET_VALUE (
                                node,
                                "bin_to_dotfile_enabled");
    char                        **_env = NULL;

    if (config == NULL || *config == NULL)
        return ;

    /* Logging redirections (stdout, stderr) */
    if (_stdout_logfile != NULL) {

        free((*config)->stdout_logfile);
        /* Good dir ? */
        (*config)->stdout_logfile = strdup(_stdout_logfile);

    } if (_stderr_logfile != NULL) {

        free((*config)->stderr_logfile);
        /* Good dir ? */
        (*config)->stderr_logfile = strdup(_stderr_logfile);

    }

    semantic_apply_config_log(*config);

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

    } if (_zmq_path != NULL) {

        free((*config)->zmq_path);
        /* ZMQ Path already used ? */
        (*config)->zmq_path = strdup(_zmq_path);

    } if (_bin_to_dotfile_enabled != NULL) {

        if (strtogboolean(_bin_to_dotfile_enabled) == TRUE)
            (*config)->bin_to_dotfile_enabled = TRUE;
        else
            (*config)->bin_to_dotfile_enabled = FALSE;

        g_print(DIRECTORY_DOTFILE_O);

    } if (_env != NULL) {

        free((*config)->env);
        (*config)->env = NULL;

    }
}

/* Config pipeline */

static void                     semantic_apply_config_pipeline(
                                ast_node_t *node,
                                config_pipeline_t **config_pipeline) {

    char                        *_init_state = AST_GET_VALUE (node, "init_state");
    char                        *_directory = AST_GET_VALUE (node, "directory");
    char                        *_type_exec = AST_GET_VALUE (node, "type_exec");
    char                        *_priority = AST_GET_VALUE (node, "priority");
    char                        *_start_secs = AST_GET_VALUE (node, "start_secs");
    char                        *_auto_restart = AST_GET_VALUE (node, "auto_restart");
    char                        *_start_datetime = AST_GET_VALUE (node, "start_datetime");

    if (config_pipeline == NULL || *config_pipeline == NULL)
        return ;

    /* Logging redirections (stdout, stderr) */
    if (_priority != NULL) {

        (*config_pipeline)->priority = atoi(_priority);

    } if (_directory != NULL) {

        if (semantic_isdir(_directory) != 0) {
            free((*config_pipeline)->directory);
            (*config_pipeline)->directory = strdup(_directory);
        } else {
            g_print(DIRECTORY_CWD_O, _directory);
        }

    } if (_init_state != NULL) {

        if (state_is_play(_init_state) == TRUE || state_is_pause(_init_state) == TRUE) {
            free((*config_pipeline)->init_state);
            (*config_pipeline)->init_state = strdup(_init_state);
        }

    } if (_type_exec != NULL) {

        if (run_is_await(_type_exec) == TRUE || run_is_thread(_type_exec) == TRUE) {
            free((*config_pipeline)->type_exec);
            (*config_pipeline)->type_exec = strdup(_type_exec);
        }

    } if (_start_secs != NULL) {

        (*config_pipeline)->start_secs = atoi(_start_secs);

    } if (_auto_restart != NULL) {

        (*config_pipeline)->auto_restart = strtogboolean(_auto_restart);

    } if (_start_datetime != NULL) {

        free((*config_pipeline)->start_datetime);
        (*config_pipeline)->start_datetime = strdup(_start_datetime);

    }

}

/* Browsing */

static void             linked_free(void *content, size_t size) {
    (void)size;
    free(content);
}

static void             semantic_apply_pipeline(
                        ast_node_t **node,
                        config_pipeline_t *config_pipeline) {

    /* used by elements_block for gst_add_bin fn */
    linked_result_t     *linked = NULL;
    GstPipeline         *pipeline = NULL;
    ast_node_t          **elements = NULL;
    ast_node_t          *tmp = *node;
    list_t              *elements_dp = lstnew("elements", sizeof(char) * 9);

    pipeline_block(node);
    pipeline = (*node)->sdata->gstpipeline;

    (*node)->config_pipeline = config_pipeline;

    elements = aast_iblock_get(
            &((*node)->left),
            elements_dp);
    if (elements != NULL) {
        (*node)->symtable = g_hash_table_new (g_str_hash, g_str_equal);
        linked = elements_block(elements, &((*node)->symtable), pipeline);
        link_element_all(linked->elements);
        link_pad_all_always(&((*node)->symtable), linked->pads);
        link_pad_all_request(&((*node)->symtable), linked->pads);
        link_pad_all_sometimes(&((*node)->symtable), linked->pads);
        lstdel(&(linked->elements), &linked_free);
        //lstdel(&(linked->pads), &linked_free);
    }
    *node = tmp;

}

supstream_t             *semantic_apply(ast_tree_t **root) {

    ast_node_t          *tmp = *root;
    ast_node_t          *priority_root = *root;
    ast_node_t          *tmp2 = *root;
    supstream_t         *supstream = (supstream_t *)malloc(sizeof(supstream_t));
    config_t            *config = config_default_new();
    config_pipeline_t   *config_pipeline = NULL;

    *root = (*root)->left;
    priority_root = *root;

    /* Priority blocks */

    while (priority_root) {
        if (AST_IBLOCK_IS (priority_root, "supstream") && priority_root->left) {
            semantic_apply_config(*root, &config);
        }
        priority_root = priority_root->right;
    }

    /* Apply each pipelines */

    while (*root) {

        tmp2 = *root;
        if (AST_IBLOCK_IS (*root, "elements") && (*root)->left) {
            *root = (*root)->left;
        } else if (AST_IBLOCK_IS (*root, "pipelines") && (*root)->left) {
            *root = (*root)->left;

            while (*root != NULL) {
                if (AST_IS_IBLOCK (*root) == TRUE) {

                    /* Specific pipeline config */

                    config_pipeline = config_pipeline_default_new();
                    semantic_apply_config_pipeline(*root, &config_pipeline);

                    semantic_apply_pipeline(root, config_pipeline);

                }
                *root = (*root)->right;
            }
        }
        *root = tmp2;
        *root = (*root)->right;

    }

    /* End ptr */

    *root = tmp;
    supstream->root = root;
    supstream->config = config;
    return (supstream);

}
