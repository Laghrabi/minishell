#include "../../minishell.h"

t_gr_cl *last_node(t_gr_cl *gar)
{
    if (!gar)
        return (NULL);
    while(gar->next)
        gar = gar->next;
    return (gar);
}

t_gr_cl *new_node(void *ptr)
{
    t_gr_cl *node;

    node = malloc (sizeof(t_gr_cl));
    if (!node)
        return (NULL);
    node->pointer = ptr;
    node->next = NULL;
    node->prev = NULL;
    return (node);
}

void free_garbage_collector(t_gr_cl *garbage)
{
    t_gr_cl *current;
    current = garbage;
    while (current)
    {
        garbage = garbage->next;
        free(current->pointer);
        free(current);
        current = garbage;
    }
}

void garbage_collector(void *new_pointer, int should_free)
{
    static t_gr_cl *garbage;
    static t_gr_cl *current;

    if (new_pointer)
    {
        t_gr_cl *new = new_node(new_pointer);
        if (!new)
            return;
        current = last_node(garbage);
        if (!current)
            garbage = new;
        else
        {
            current->next = new;
            new->prev = current;
        }
    }
    if (should_free == 1)
    {
        free_garbage_collector(garbage);
        garbage = NULL;
        current = NULL;
    }
}

void *gc_malloc(size_t size)
{
    void *ptr = malloc(size);
    if (!ptr)
        return NULL;
    garbage_collector(ptr, 0);
    return ptr;
}