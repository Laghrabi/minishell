/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:04:58 by zfarouk           #+#    #+#             */
/*   Updated: 2025/05/30 12:08:13 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char *check_var(char *token, t_env *env)
{
    size_t i;
    char *var_name;

    i = 0;
    if (token[0] == ' ' || token[i] == '\t' || token[i] == '\0' || token[i] == '\"' || token[i] == '\'')
        return (ft_strdup("$"));
    while (token[i] != ' ' && token[i] != '\t' && token[i] != '\0'&& token[i] != '\"' && token[i] != '\'')
        i++;
    var_name = ft_substr(token, 0, i);
    return (get_env_value(var_name, env));
}

void fix_value(char *value)
{
    int (i), (j);
    int in_space;

    i = 0;
    j = 0;
    in_space = 0;
    while (value[i] == ' ' || value[i] == '\t')
        i++;
    while (value[i])
    {
        if (value[i] == ' ' || value[i] == '\t')
        {
            if (!in_space)
                value[j++] = ' ';
            in_space = 1;
        }
        else
        {
            value[j++] = value[i];
            in_space = 0;
        }
        i++;
    }
    if (j > 0 && value[j - 1] == ' ')
        j--;
    value[j] = '\0';
}

int  write_value(char **token, char *value, int *field, db_int yes)
{
    int len;
    int i;
    char *new_token;
    
    if (field[yes.i] == 4)
        fix_value(value);
    len = ft_strlen(value);
    new_token = gc_malloc((yes.len + len + 1) * sizeof(char));
    len = 0;
    while ((*token)[len])
    {
        new_token[len] = (*token)[len];
        len++;
    }
    i = -1;
    while (value[++i])
        new_token[len + i] = value[i];
    *token = new_token;
    return (len + i);
}

char *expanded_token(char *token, int *field, t_env *env)
{
    int (i), (j), (len);
    char *new_token;
    char *value;

    len = ft_strlen(token);
    new_token = gc_malloc(len * sizeof(char) + 1);
    i = -1;
    j = 0;
    while (token[++i])
    {
        if (extra_quote(token[i], field[i]))
            continue;
        if (token[i] == '$' && field[i] != 2)
        {
            value = check_var(&token[i+1], env);
            if (value)
                j = write_value(&new_token, value, field, (db_int){i, len});
            else
                skip_var(token, &i);
        }
        else
            new_token[j++] = token[i]; 
    }
    new_token[j] = '\0';
    return (new_token);
}

void expansion(t_token *arg_list, t_env *env)
{
    int *field;

    while (arg_list)
    {
        field = set_field(arg_list->value);
		arg_list->value = expanded_token(arg_list->value, field, env);
        arg_list = arg_list->next;
    }
}