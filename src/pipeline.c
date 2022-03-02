#include "pipeline.h"

static void         exec_initial_state(
                    GstPipeline *pipeline,
                    config_pipeline_t *config_pipeline) {

    if (state_is_play(config_pipeline->init_state) == TRUE)
        gst_element_set_state ((GstElement *)pipeline, GST_STATE_PLAYING);
    else if (state_is_pause(config_pipeline->init_state) == TRUE)
        gst_element_set_state ((GstElement *)pipeline, GST_STATE_PAUSED);
    else
        gst_element_set_state ((GstElement *)pipeline, GST_STATE_PLAYING);
}

/* Type of running (await/thread) */

static void         *exec_bus(void *_execdata) {

    execdata_t      *execdata = (execdata_t *)_execdata;
    GstBus          *bus = NULL;
    int             *bus_ld;
    int             (*bus_ld_fn)(GstBus *, GstPipeline *) = NULL;

    bus = gst_element_get_bus((GstElement *)execdata->pipeline);

    /* Use default bus function */

    if (execdata->bus_fn == NULL) {
        default_bus(bus, execdata->pipeline);
        return (NULL);
    }

    /* Use bus plugin function */

    if (!(bus_ld = dlopen(execdata->plugin, RTLD_LAZY))) {
        printf ("Supstream: unfound plugin: %s\n", dlerror ());
        exit (EXIT_FAILURE);
    }
    if (!(bus_ld_fn = dlsym (bus_ld, execdata->bus_fn))) {
        printf ("Supstream: unfound bus function: %s\n", dlerror ());
        dlclose (bus_ld);
        exit (EXIT_FAILURE);
    }
    bus_ld_fn (bus, execdata->pipeline);
    dlclose (bus_ld);

    return (NULL);
}

static void         exec_run_dotfile(execdata_t *execdata) {

    /* Generate graph vizualization */
    if (execdata->config->bin_to_dotfile_enabled == TRUE) {
        GST_DEBUG_BIN_TO_DOT_FILE(
                GST_BIN (execdata->pipeline),
                GST_DEBUG_GRAPH_SHOW_ALL,
                GST_ELEMENT_NAME (execdata->pipeline));
    }
}

static pthread_t    exec_run_await(execdata_t *execdata) {

    g_print(PIPELINE_RUN_AWAIT_O, GST_ELEMENT_NAME (execdata->pipeline));

    exec_run_dotfile(execdata);
    exec_bus(execdata);
    return (0);
}

static pthread_t    exec_run_thread(execdata_t *execdata) {

    pthread_t       thread_id;
    int             ret;

    g_print(PIPELINE_RUN_THREAD_O, GST_ELEMENT_NAME (execdata->pipeline));

    exec_run_dotfile(execdata);
    ret = pthread_create(&thread_id, NULL, exec_bus, (void *)execdata);
    if (ret != 0) {
        g_printerr(PIPELINE_ERROR_RUN_THREAD_O,
                GST_ELEMENT_NAME (execdata->pipeline));
    }

    return (thread_id);
}

static void         exec_pipeline_run(
                    ast_node_t **node,
                    execdata_t *execdata) {

    ast_node_t      *run = ast_iscalar_get_by_key(*node, "type_exec");
    pthread_t       pt_id;

    if (AST_RCHILD(run) && run_is_await(run->right->str) == TRUE) {
        pt_id = exec_run_await(execdata);
    } else if (AST_RCHILD(run) && run_is_thread(run->right->str) == TRUE) {
        pt_id = exec_run_thread(execdata);
        (*node)->pthread_id = pt_id;
    } else {
        pt_id = exec_run_thread(execdata);
        (*node)->pthread_id = pt_id;
    }
}

/* Pipeline execution */

static void         exec_pipeline_exec_type(
                    ast_node_t **root,
                    execdata_t *execdata,
                    gboolean (*fn)(gchar *)) {

    list_t          *deepblock = ast_deepblock_create(2, "document", "pipelines");
    semantic_data_t *sdata;
    GstPipeline     *pipeline = NULL;
    int             ret = -1;
    ast_node_t      *root_ptr = *root;

    *root = ast_iblock_get(*root, deepblock);
    *root = (*root)->left;

    while (*root) {

        if (ast_node_is_iblock(*root)
                && fn((*root)->config_pipeline->type_exec) == TRUE) {
            
            /* Pipeline config init */

            ret = chdir((*root)->config_pipeline->directory);
            if (ret == -1)
                return ;

            /* Pipeline init */

            sdata = (semantic_data_t *)(*root)->sdata;
            pipeline = (GstPipeline *)sdata->gstpipeline;
            execdata->pipeline = pipeline;
            exec_initial_state(pipeline, (*root)->config_pipeline);
            exec_pipeline_run(root, execdata);

        }
        *root = (*root)->right;
    }

    *root = root_ptr;

    ast_deepblock_free(deepblock);

}

static void         *exec_pipeline_exec_type_pthread(void *privdata) {

    privdata_sync_t *_privdata = (privdata_sync_t *)privdata;
    ast_tree_t      **root = (ast_tree_t **)_privdata->root;
    execdata_t      *execdata = (execdata_t *)_privdata->execdata;
    gboolean        (*fn)(gchar *) = _privdata->fn;
    list_t          *deepblock = ast_deepblock_create(2, "document", "pipelines");
    semantic_data_t *sdata;
    GstPipeline     *pipeline = NULL;
    int             ret = -1;
    ast_node_t      *root_ptr = *root;
    int             i = 1;

    /* Priority */

    *root = ast_iblock_get(*root, deepblock);
    *root = (*root)->left;

    while (*root) {

        if (ast_node_is_iblock(*root)
                && (*root)->config_pipeline->priority == i
                && fn((*root)->config_pipeline->type_exec) == TRUE) {

            /* Pipeline config init */

            ret = chdir((*root)->config_pipeline->directory);
            if (ret == -1)
                return (NULL);

            /* Pipeline init */

            sdata = (semantic_data_t *)(*root)->sdata;
            pipeline = (GstPipeline *)sdata->gstpipeline;
            execdata->pipeline = pipeline;
            exec_initial_state(pipeline, (*root)->config_pipeline);
            exec_pipeline_run(root, execdata);

            i++;

            *root = root_ptr;
            *root = ast_iblock_get(*root, deepblock);
            *root = (*root)->left;

        } else {

            *root = (*root)->right;

        }
    }

    /* Others */

    *root = root_ptr;
    *root = ast_iblock_get(*root, deepblock);
    *root = (*root)->left;

    while (*root) {

        if (ast_node_is_iblock(*root)
                && (*root)->config_pipeline->priority == CONFIG_PIPELINE_UNDEFINED_PRIORITY
                && fn((*root)->config_pipeline->type_exec) == TRUE) {
            
            /* Pipeline config init */

            ret = chdir((*root)->config_pipeline->directory);
            if (ret == -1)
                return (NULL);

            /* Pipeline init */

            sdata = (semantic_data_t *)(*root)->sdata;
            pipeline = (GstPipeline *)sdata->gstpipeline;
            execdata->pipeline = pipeline;
            exec_initial_state(pipeline, (*root)->config_pipeline);
            exec_pipeline_run(root, execdata);

        }
        *root = (*root)->right;
    }


    *root = root_ptr;

    ast_deepblock_free(deepblock);

    return (NULL);
}

void                exec_pipeline(supstream_t *supstream) {

    list_t          *deepblock = ast_deepblock_create(2, "document", "pipelines");
    ast_tree_t      **root = supstream->root;
    ast_node_t      *tmp_join = ast_iblock_get(*root, deepblock);
    pthread_t       thread_gateway_id;
    pthread_t       thread_sync_id;
    ast_node_t      *pipelines = ast_iblock_get(*root, deepblock);
    execdata_t      *execdata = (execdata_t *)malloc(sizeof(execdata_t));
    gatewaydata_t   *gatewaydata =(gatewaydata_t *)malloc(sizeof(gatewaydata_t));
    ast_node_t      *plugin_node = ast_iscalar_get_by_key(pipelines, "plugin");
    ast_node_t      *bus_fn_node = ast_iscalar_get_by_key(pipelines, "bus_fn");
    privdata_sync_t *privdata_sync = (privdata_sync_t *)malloc(sizeof(privdata_sync_t));

    int             ret;

    /* Init plugin & bus_fn privdata struct correctly */

    execdata->config = supstream->config;

    if (plugin_node != NULL
            && bus_fn_node != NULL
            && plugin_node->right
            && bus_fn_node->right) {
        execdata->plugin = plugin_node->right->str;
        execdata->bus_fn = bus_fn_node->right->str;
    } else {
        execdata->plugin = NULL;
        execdata->bus_fn = NULL;
    }

    /* GATEWAY, for zmq_disabled = False */

    if (execdata->config->zmq_disabled == FALSE) {
        gatewaydata->root = root;
        gatewaydata->config = supstream->config;
        ret = pthread_create(&thread_gateway_id, NULL, gateway, gatewaydata);
        if (ret != 0)
            return ;
    }

    /* THREAD type_exec */

    exec_pipeline_exec_type(root, execdata, &run_is_thread);

    /* SYNC type_exec */

    if (privdata_sync == NULL)
        return ;

    privdata_sync->root = root;
    privdata_sync->execdata = execdata;
    privdata_sync->fn = &run_is_await;
    ret = pthread_create(&thread_sync_id, NULL, exec_pipeline_exec_type_pthread, (void *)privdata_sync);
    if (ret != 0) {
        g_printerr(PIPELINE_ERROR_RUN_THREAD_O,
                GST_ELEMENT_NAME (execdata->pipeline));
    }

    /* Join GATEWAY / THREAD / SYNC */

    tmp_join = tmp_join->left;

    while (tmp_join) {

        if (ast_node_is_iblock(tmp_join)
                && run_is_thread(tmp_join->config_pipeline->type_exec) == TRUE
                && tmp_join->pthread_id != 0) {
            pthread_join(tmp_join->pthread_id, NULL);
        }
        tmp_join = tmp_join->right;

    }

    pthread_join(thread_sync_id, NULL);
    pthread_join(thread_gateway_id, NULL);

    ast_deepblock_free(deepblock);

}
