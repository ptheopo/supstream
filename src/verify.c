#include "verify.h"

gboolean    strtogboolean(gchar *str) {

    if (str
            && (strcmp(str, "y") == 0
                || strcmp(str, "Y") == 0
                || strcmp(str, "yes") == 0
                || strcmp(str, "Yes") == 0
                || strcmp(str, "YES") == 0
                || strcmp(str, "true") == 0
                || strcmp(str, "True") == 0
                || strcmp(str, "TRUE") == 0
                || strcmp(str, "on") == 0
                || strcmp(str, "On") == 0
                || strcmp(str, "ON") == 0))
        return (TRUE);
     else if (str
             && (strcmp(str, "n") == 0
                 || strcmp(str, "N") == 0
                 || strcmp(str, "no") == 0
                 || strcmp(str, "No") == 0
                 || strcmp(str, "NO") == 0
                 || strcmp(str, "false") == 0
                 || strcmp(str, "False") == 0
                 || strcmp(str, "FALSE") == 0
                 || strcmp(str, "off") == 0
                 || strcmp(str, "Off") == 0
                 || strcmp(str, "OFF") == 0))
         return (FALSE);

     return (FALSE);

}

gboolean     run_is_await(gchar *str) {

    if (str
            && (strcmp(str, "sync") == 0
                || strcmp(str, "Sync") == 0
                || strcmp(str, "SYNC") == 0))
        return (TRUE);
    return (FALSE);
}

gboolean     run_is_thread(gchar *str) {

    if (str
            && (strcmp(str, "thread") == 0
                || strcmp(str, "Thread") == 0
                || strcmp(str, "THREAD") == 0))
        return (TRUE);
    return (FALSE);
}

gboolean     state_is_play(gchar *str) {

    if (str
            && (strcmp(str, "play") == 0
                || strcmp(str, "Play") == 0
                || strcmp(str, "PLAY") == 0
                || strcmp(str, "playing") == 0
                || strcmp(str, "Playing") == 0
                || strcmp(str, "PLAYING") == 0))
        return (TRUE);
    return (FALSE);
}

gboolean     state_is_pause(gchar *str) {

    if (str
            && (strcmp(str, "pause") == 0
                || strcmp(str, "Pause") == 0
                || strcmp(str, "PAUSE") == 0
                || strcmp(str, "paused") == 0
                || strcmp(str, "Paused") == 0
                || strcmp(str, "PAUSED") == 0))
        return (TRUE);
    return (FALSE);
}
