#include "fifo.h"

fifo_t      *fifo_new(char *str) {

    fifo_t  *fifo = (fifo_t *)malloc(sizeof(fifo_t));

    if (fifo == NULL) return (NULL);

    if (str != NULL) fifo->str = strdup(str);
    else fifo->str = NULL;
    fifo->next = NULL;

    return (fifo);
}

void        fifo_push(fifo_t **fifo, char *str) {

    fifo_t  *new_fifo = fifo_new(str);
    fifo_t  *tmp = *fifo;

    if (*fifo != NULL) {

        while ((*fifo)->next)
            *fifo = (*fifo)->next;
        (*fifo)->next = new_fifo;

        *fifo = tmp;

    } else {
        *fifo = new_fifo;
    }
}

char        *fifo_pop(fifo_t **fifo) {

    fifo_t  *new_heap = NULL;
    char    *str_token = NULL;

    if (*fifo == NULL) return (NULL);
    if ((*fifo)->next) new_heap = (*fifo)->next;

    str_token = (*fifo)->str;
    free(*fifo);
    *fifo = new_heap;

    return (str_token);
}

void        fifo_free(fifo_t **fifo) {

    fifo_t  *next = NULL;

    while (*fifo) {
        next = (*fifo)->next;
        free(*fifo);
        *fifo = next;
    }
    *fifo = NULL;
}

int         fifo_count(fifo_t *fifo) {

    int     i = 0;

    while (fifo != NULL) {
        i += 1;
        fifo = fifo->next;
    }
    return (i);
}

fifo_t      *fifo_dup(fifo_t *fifo) {

    fifo_t  *fifo_dup = NULL;

    if (fifo == NULL) return (NULL);
    while (fifo != NULL) {
        fifo_push(&fifo_dup, fifo->str);
        fifo = fifo->next;
    }
    return (fifo_dup);
}

void        fifo_print(fifo_t *fifo) {

    while (fifo && fifo->next) {
        g_print("%s\n", fifo->str);
        fifo = fifo->next;
    }
    g_print("\n\n");
}
