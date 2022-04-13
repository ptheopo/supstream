#include "semantic.h"

void                    semantic_line_set_delay_cb(int period, void *arg) {

    (void)period;
    GstElement          *element = (GstElement *)arg;

    gst_element_set_state(GST_ELEMENT (element), GST_STATE_PLAYING);
}

void                    semantic_line_set_delay(
                        GstElement *element,
                        char *delay) {
    
    struct                      tm tm;
    time_t                      time_delay;
    time_t                      current;
    double                      diff;
    char                        *ret = 0;

    if (delay == NULL)
        return ;

    /* Get set_delay and current timestamp */
    ret = strptime(delay, "%Y-%m-%d %H:%M:%S", &tm);
    if (ret == NULL) {
        g_printerr(SEMANTIC_ERROR_DATETIME_FORMAT_O);
        return ;
    }
    time_delay = mktime(&tm) + timezone;
    current = time(NULL) + timezone;

    diff = difftime(time_delay, current);
    if (diff >= 0) {

        /* Set delay */
        pev_timer_add(diff * 10000, diff * 10000, semantic_line_set_delay_cb, (void *)element);
        g_print(SEMANTIC_DATETIME_O, GST_ELEMENT_NAME(element), delay);

    } else
        g_printerr(SEMANTIC_ERROR_DATETIME_O);

}
