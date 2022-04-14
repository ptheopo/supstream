#include "semantic.h"

static void             semantic_line_start_playing_cb(int period, void *arg) {

    (void)period;
    GstElement          *element = (GstElement *)arg;

    gst_element_set_state(element, GST_STATE_PLAYING);
    g_print(SCHEDULER_PLAYING_O, GST_ELEMENT_NAME (element));
}

static void             semantic_line_start_paused_cb(int period, void *arg) {

    (void)period;
    GstElement          *element = (GstElement *)arg;

    gst_element_set_state(element, GST_STATE_PAUSED);
    g_print(SCHEDULER_PLAYING_O, GST_ELEMENT_NAME (element));
}

static void             semantic_line_start_null_cb(int period, void *arg) {

    (void)period;
    GstElement          *element = (GstElement *)arg;

    gst_element_set_state(element, GST_STATE_NULL);
    g_print(SCHEDULER_PLAYING_O, GST_ELEMENT_NAME (element));
}

static void             semantic_line_start_ready_cb(int period, void *arg) {

    (void)period;
    GstElement          *element = (GstElement *)arg;

    gst_element_set_state(element, GST_STATE_READY);
    g_print(SCHEDULER_PLAYING_O, GST_ELEMENT_NAME (element));
}

void                    semantic_line_start_playing(
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
        pev_timer_add(diff * 1000000, 0, semantic_line_start_playing_cb, (void *)element);
        g_print(SEMANTIC_DATETIME_O, GST_ELEMENT_NAME(element), delay);

    } else
        g_printerr(SEMANTIC_ERROR_DATETIME_O);

}

void                    semantic_line_start_paused(
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
        pev_timer_add(diff * 1000000, 0, semantic_line_start_paused_cb, (void *)element);
        g_print(SEMANTIC_DATETIME_O, GST_ELEMENT_NAME(element), delay);

    } else
        g_printerr(SEMANTIC_ERROR_DATETIME_O);

}

void                    semantic_line_start_null(
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
        pev_timer_add(diff * 1000000, 0, semantic_line_start_null_cb, (void *)element);
        g_print(SEMANTIC_DATETIME_O, GST_ELEMENT_NAME(element), delay);

    } else
        g_printerr(SEMANTIC_ERROR_DATETIME_O);

}

void                    semantic_line_start_ready(
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
        pev_timer_add(diff * 1000000, 0, semantic_line_start_ready_cb, (void *)element);
        g_print(SEMANTIC_DATETIME_O, GST_ELEMENT_NAME(element), delay);

    } else
        g_printerr(SEMANTIC_ERROR_DATETIME_O);

}
