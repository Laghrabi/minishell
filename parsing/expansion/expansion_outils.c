/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_outils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 20:02:07 by zfarouk           #+#    #+#             */
/*   Updated: 2025/05/31 16:48:09 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int which_type(char c, int *db_quote, int *sg_quote, int *quote_state)
{
    if (c == '\'')
    {
        if (*db_quote == 1)
            return (*quote_state);
        else if(*sg_quote == 1)
        {
            *sg_quote = 0;
            *quote_state = 0;
        }
        else if (*sg_quote == 0)
        {
            *sg_quote = 1;
            *quote_state = 2;
			return (0);
        }
    }
    else if (c == '\"')
    {
        if (*sg_quote == 1)
            return (*quote_state);
        if (*db_quote == 1)
        {
            *db_quote = 0;
            *quote_state = 0;
        }
        else if (*db_quote == 0)
        {
            *db_quote = 1;
            *quote_state = 4;
			return (0);
        }
    }
    return (*quote_state);
}

char    *get_env_value(char *key, t_env *env_list)
{
    t_env   *current;
    
    if (key == NULL || env_list == NULL)
        return (NULL);
    current = env_list;
    while (current != NULL)
    {
		if (ft_strcmp(key, current->key) == 0)
			return (current->value);
		current = current->next;
    }
	return (NULL);
}

int *set_field(char *str)
{
    int *field;
    int (i), (db_quote), (sg_quote);
    int type;
    int quote_state;
    
    i = 0;
	db_quote = 0;
	sg_quote = 0;
    quote_state = 0;
    field = gc_malloc((ft_strlen(str)) * sizeof(int));
    while (str[i])
    {
		type = which_type(str[i], &db_quote, &sg_quote, &quote_state);
        field[i++] = type;
    }
	return field;
} 

void skip_var(char *token, int *i)
{
    (*i)++;
    while (ft_isalnum(token[*i]) || token[*i] == '_')
        (*i)++;
    (*i)--;
}

int extra_quote(char c, int i)
{
    if ((c == '\"' || c == '\'') && i == 0)
        return (1);
    return (0);
}