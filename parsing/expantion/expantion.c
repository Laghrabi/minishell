/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expantion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:04:58 by zfarouk           #+#    #+#             */
/*   Updated: 2025/05/28 16:25:22 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int wich_type(char c, int *db_quote, int *sg_quote)
{
    static char i = 0;

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

int *set_field(char *str)
{
    char *field;
    int (i), (db_quote), (sg_quote);
    char type;
    
    i = 0;
	db_quote = 0;
	sg_quote = 0;
    field = gc_malloc((ft_strlen(str) + 1) * sizeof(int));
    while (str[i])
    {
		type = which_type(str[i], &db_quote, &sg_quote);
        field[i++] = type;
    }
	field[i] = '\0';
	return field;
} 
int extra_quote(char c, int i)
{
    if ((c == '\"' || c == '\'') && i == 0)
        return (1);
    return (0);
}

char *expanded_token(char *token, char *field, t_env *env)
{
    int (i), (j), (len), (quote);
    char *new_token;

    len = ft_strlen(token);
    new_token = gc_malloc(len * sizeof(char) + 1);
    i = -1;
    j = 0;
    while (token[++i])
    {
        if (extra_quote(token[i], field[i]))
            continue;
        if (token[i] == '$')
            check_var
    }
    
}

void expantion(t_token *arg_list, t_env *env)
{
    int *field;

    while (arg_list)
    {
        field = set_field(arg_list->value);
		arg_list->value = expanded_token(arg_list->value, field, env);
        arg_list = arg_list->next;
    }
}