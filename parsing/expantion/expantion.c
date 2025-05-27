/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expantion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:04:58 by zfarouk           #+#    #+#             */
/*   Updated: 2025/05/27 18:00:48 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int wich_type(char c, int *db_quote, int *sg_quote)
{
    static int i;

    if (c == '\'')
    {
        if (*db_quote == 1)
            return (i);
        else if(*sg_quote == 1)
        {
            *sg_quote = 0;
            i = 0;
        }
        else if (*sg_quote == 0)
        {
            *sg_quote = 1;
            i = 2;
			return (0);
        }
    }
    else if (c == '\"')
    {
        if (*sg_quote == 1)
            return (i);
        if (*db_quote == 1)
        {
            *db_quote = 0;
            i = 0;
        }
        else if (*db_quote == 0)
        {
            *db_quote = 1;
            i = 4;
			return (0);
        }
    }
    return (i);
}

char *set_field(char *str)
{
    char *field;
    int (i), (db_quote), (sg_quote), (type);

    i = 0;
	db_quote = 0;
	sg_quote = 0;
    field = gc_malloc(ft_strlen(str) + 1);
    while (str[i])
    {
		type = which_type(str[i], &db_quote, &sg_quote);
        field[i++] = (char)type;
    }
	field[i] = '\0';
	return field;
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