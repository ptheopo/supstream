# include "bus.h"

int             bus_core(
                GstBus *bus,
                GstPipeline *pipeline,
                config_pipeline_t *config_pipeline) {

    GstMessage  *msg = NULL;

    /* Message */
    msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_EOS);
    if (GST_MESSAGE_TYPE (msg) == GST_MESSAGE_ERROR) {
        g_error ("An error occurred! Re-run with the GST_DEBUG=*:WARN environment "
                "variable set for more details.");
    }

    /* Play */
    gst_element_set_state(GST_ELEMENT (pipeline), GST_STATE_PLAYING);

    /* Bus manager */
    switch (GST_MESSAGE_TYPE (msg)) {
        case GST_MESSAGE_EOS:

            gst_message_unref (msg);

            if (config_pipeline->auto_restart == TRUE) {

                while (42) {
                    gst_element_set_state(GST_ELEMENT (pipeline), GST_STATE_NULL);
                    gst_element_set_state(GST_ELEMENT (pipeline), GST_STATE_READY);
                    gst_element_set_state(GST_ELEMENT (pipeline), GST_STATE_PLAYING);
                }

            } else {

                gst_element_set_state(GST_ELEMENT (pipeline), GST_STATE_NULL);
                gst_object_unref (bus);
                gst_object_unref (pipeline);

            }

            break ;

        default:
            break ;

    }

    return (0);
}
