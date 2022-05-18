#ifndef FIFO_H
# define FIFO_H

# include <glib.h>
# include <stdlib.h>
# include <string.h>

/*
 * FIFO is NULL if empty
 */

typedef struct      fifo_s {
    char            *str;
    struct fifo_s   *next;
}                   fifo_t;


fifo_t              *fifo_new(char *str);
void                fifo_push(
                    fifo_t **fifo,
                    char *str);
char                *fifo_pop(fifo_t **fifo);
void                fifo_free(fifo_t **fifo);
int                 fifo_count(fifo_t *fifo);
fifo_t              *fifo_dup(fifo_t *fifo);
void                fifo_print(fifo_t *fifo);

#endif


