# include "default.h"

int             default_bus(GstBus *bus, GstPipeline *pipeline) {

    GstMessage  *msg = NULL;

    msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_EOS);
    if (GST_MESSAGE_TYPE (msg) == GST_MESSAGE_ERROR) {
        g_error ("An error occurred! Re-run with the GST_DEBUG=*:WARN environment "
                "variable set for more details.");
    }
    gst_element_set_state (GST_ELEMENT (pipeline), GST_STATE_NULL);
    gst_message_unref (msg);
    gst_object_unref (bus);
    gst_object_unref (pipeline);
    return (0);
}
