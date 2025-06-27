/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 03:44:43 by claghrab          #+#    #+#             */
/*   Updated: 2025/06/27 03:58:46 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char **token_list_to_argv(t_token *token_list)
{
    char **argv;
    t_token *current;
    int i;
    
    if (token_list == NULL)
        return (NULL);
    i = 0;
    current = token_list;
    while (current != NULL)
    {
        i++;
        current = current->next;
    }
    argv = gc_malloc(sizeof(char *) * (i + 1));
    i = 0;
    current = token_list;
    while (current != NULL)
    {
        argv[i++] = ft_strdup(current->value);
        current = current->next;
    }
    argv[i] = NULL;
    return (argv);
}
