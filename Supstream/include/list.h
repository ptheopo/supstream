#ifndef LIST_H
# define LIST_H

# include <string.h>
# include <stdlib.h>

typedef struct      s_list
{
    void            *content;
    size_t          content_size;
    struct s_list   *next;
}                   list_t;

list_t              *lstnew(
                    void const *content,
                    size_t content_size);
void                lstdelone(
                    list_t **alst,
                    void (*del)(void *, size_t));
void                lstdel(
                    list_t **alst,
                    void (*del)(void *, size_t));
list_t              *lstmap(
                    list_t *lst,
                    list_t *(*f)(list_t *elem));
void	            lstiter(
                    list_t *lst,
                    void (*f)(list_t *elem));
void                lstadd(
                    list_t **alst,
                    list_t *new);
void                lstdellast(list_t **alst);
void	            lstaddlast(
                    list_t **alst,
                    list_t *new);
list_t	            *lstid(
                    list_t *lst,
                    unsigned int n);
list_t	            *lstlast(list_t *lst);

#endif
