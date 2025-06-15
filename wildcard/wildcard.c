/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 18:04:27 by zfarouk           #+#    #+#             */
/*   Updated: 2025/06/14 19:59:00 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void check_wildcard(t_token **token)
{
    int i;
    if (!token || !(*token) || ((*token)->field && (*token)->field != 0 && (*token)->field != 1))
        return;
    i = 0;
    while ((*token)->value[i])
    {
        if ((*token)->value[i] == '*')
        {
            matches = search_matches((*token)->value);
            return
        }
    }
}

void wildcard(t_token **arg_list)
{
    t_token *current;
    if (arg_list == NULL || arg_list == NULL)
        return;
    current = *arg_list;
    while (current)
    {
        check_wildcard(&current);
        if (current->pre == NULL)
            *arg_list = current;
        current = current->next;
    }
}