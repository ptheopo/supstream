#ifndef MAIN_H
# define MAIN_H

# include <assert.h>
# include <glib.h>
# include <locale.h>
# include <string.h>
# include <termios.h>
# include <unistd.h>
# include <yaml.h>

# include "ast.h"
# include "def.h"
# include "fifo.h"
# include "pipeline.h"
# include "semantic.h"

# define    FAILED_OPS 0
# define    SUCCESS_OPS 1
# define    FAILED_OPEN_YPIP_OPS -1
# define    FAILED_INIT_YPIP_OPS -2

#endif
