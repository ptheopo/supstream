#ifndef DEFAULT_H
# define DEFAULT_H

# include <gst/gst.h>

/* Default function are used if there is no specified plugin functions */

int default_bus(GstBus *bus, GstPipeline *pipeline);

#endif
