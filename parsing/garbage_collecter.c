#include "../minishell.h"

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

void free_garbage_collecter(t_gr_cl *garbage)
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

void garbage_collecter(void *new_pointer, int  should_free)
{
    static t_gr_cl *garbage = NULL;
    static t_gr_cl *current;

    if (new_pointer)
    {
        current = last_node(garbage);
        if (!current)
            garbage = new_node(new_pointer);
        else
        {
            current->next = new_node(new_pointer);
            current->next->prev = current;
        }
    }
    if (should_free == 1)
        free_garbage_collecter(garbage);
}

void *gc_malloc(size_t size)
{
    void *ptr = malloc(size);
    garbage_collecter(ptr, 0);
    return ptr;
}