#ifndef VERIFY_H
# define VERIFY_H

# include <string.h>

# include "ast.h"

gboolean    strtogboolean(gchar *str);
gboolean    run_is_await(gchar *str);
gboolean    run_is_thread(gchar *str);
gboolean    state_is_play(gchar *str);
gboolean    state_is_pause(gchar *str);
gboolean    state_is_ready(gchar *str);
gboolean    state_is_null(gchar *str);

#endif