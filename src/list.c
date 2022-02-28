#include "list.h"

static void         *memdup(void const *dest, size_t n)
{
	void            *dup = NULL;

	if (!(dup = malloc(sizeof(unsigned char) * n)))
		return (NULL);
	memcpy(dup, dest, n);
	return (dup);
}

void                lstadd(list_t **alst, list_t *new)
{
    if (alst && *alst && new)
    {
        new->next = *alst;
        *alst = new;
    }
    else if (!alst && !(*alst) && new)
        *alst = new;
}

void                lstaddlast(list_t **alst, list_t *new)
{
    list_t          *tmp = NULL;

    tmp = *alst;
    if (!alst || !*alst)
        *alst = new;
    else
    {
        while (tmp->next) tmp = tmp->next;
        tmp->next = new;
    }
}

void                lstdel(list_t **alst, void (*del)(void *, size_t))
{
    list_t          *lst = NULL;
    list_t          *next = NULL;

    lst = *alst;
    while (lst)
    {
        next = lst->next;
        lstdelone(&lst, del);
        lst = next;
    }
    *alst = NULL;
}

void                lstdelone(list_t **alst, void (*del)(void *, size_t))
{
    if (alst)
    {
        if (del)
            del((*alst)->content, (*alst)->content_size);
        free(*alst);
        *alst = NULL;
    }
}

list_t              *lstid(list_t *lst, unsigned int n)
{
    list_t          *tmp = lst;
    unsigned int    i = 0;

    while (tmp && i < n)
    {
        tmp = tmp->next;
        i++;
    }
    return (tmp);
}

void                lstiter(list_t *lst, void (*f)(list_t *elem))
{
    list_t          *tmp = lst;

    while (tmp)
    {
        f(tmp);
        tmp = tmp->next;
    }
}

list_t              *lstlast(list_t *lst)
{
    list_t          *tmp = lst;

    while (tmp->next) tmp = tmp->next;
    return (tmp);
}

list_t              *lstmap(list_t *lst, list_t *(*f)(list_t *elem))
{
    list_t          *result = NULL;
    list_t          *tmp = NULL;
    list_t          *tmp2 = NULL;

    if (!lst || !f) return (NULL);

    tmp2 = f(lst);
    if ((result = lstnew(tmp2->content, tmp2->content_size)))
    {

        tmp = result;
        lst = lst->next;
        while (lst)
        {

            tmp2 = f(lst);
            if (!(tmp->next = lstnew(tmp2->content, tmp2->content_size)))
                return (NULL);
            tmp = tmp->next;
            lst = lst->next;

        }

    }
    return (result);
}

list_t              *lstnew(void const *content, size_t content_size)
{
    list_t          *new = NULL;

    if (!(new = (list_t *)malloc(sizeof(list_t)))) return (NULL);

    new->content = (content == NULL)
        ? NULL : memdup(content, content_size);
    new->content_size = (content == NULL)
        ? 0 : content_size;
    new->next = NULL;
    return (new);
}

void                lstdellast(list_t **alst) {

    list_t          *tmp = *alst;
    list_t          *last = NULL;

    if (!alst || !(*alst)) return ;

    while (tmp->next && tmp->next->next) tmp = tmp->next;
    last = tmp->next;
    tmp->next = NULL;
    if (last && last->content) {
        free(last->content);
        free(last);
    }
}
