#ifndef DEFAULT_H
# define DEFAULT_H

# include "config.h"

# include <gst/gst.h>

int bus_core(
    GstBus *bus,
    GstPipeline *pipeline,
    config_pipeline_t *config_pipeline);

#endif
