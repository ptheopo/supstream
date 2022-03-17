#include "semantic.h"

void                    pipeline_block(ast_node_t **node) {

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
