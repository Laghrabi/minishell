/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extantion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-26 16:04:58 by zfarouk           #+#    #+#             */
/*   Updated: 2025-05-26 16:04:58 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *set_field(char *str)
{
    char *field;
    
    field = gc_malloc(ft_strlen(str) + 1);
    while (str[i])
    {
        
    }
} 

void expantion(t_token *arg_list)
{
    char *field;

    while (arg_list)
    {
        field = set_field(arg_list->value);
        arg_list = arg_list->next;
    }
}