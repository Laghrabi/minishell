/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:04:58 by zfarouk           #+#    #+#             */
/*   Updated: 2025/05/31 16:50:47 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char *check_var(char *token, t_env *env, int *index)
{
    size_t i;
    char *var_name;
    char *value;

    i = 0;
    /* should handle $?
        if (token[0] == '?')
            ...    
    */
        
    if (ft_isalpha(token[0]) || token[0] == '_')
    {
        while (ft_isalnum(token[i]) || token[i] == '_')
            i++;
        var_name = ft_substr(token, 0, i);
        value = get_env_value(var_name, env);
        if (value)
            *index = *index + ft_strlen(var_name);
    }
    else 
    {
        *index = *index + 1;
        return (ft_strdup("$"));
    }
    return (value);
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

void write_value(t_extoken *ex_token, char *value, int *field, int yes)
{
    int len;
    int i;
    int j;
    char *new_token;
    
    if (field[yes] == 0)
        fix_value(value);
    len = ft_strlen(value);
    new_token = gc_malloc((ex_token->buffer_size - ex_token->removed_quote + len + 1) * sizeof(char));
    ex_token->removed_quote = 0;
    i = -1;
    while (++i < ex_token->index)
        new_token[i] = ex_token->new_token[i];
    j = 0;
    while(value[j])
    {
        new_token[i + j] = value[j];
        j++;
    }
    ex_token->index = j + i;
    ex_token->new_token = new_token;
}

char *expanded_token(char *token, int *field, t_env *env)
{
    int i;
    t_extoken *ex_token;
    char *value;

    ex_token = gc_malloc(sizeof(t_extoken));
    ex_token->buffer_size = ft_strlen(token);
    ex_token->new_token = gc_malloc(ex_token->buffer_size * sizeof(char) + 1);
    ex_token->removed_quote = 0;
    ex_token->index = 0;
    i = -1;
    while (token[++i])
    {
        if (extra_quote(token[i], field[i]))
            continue;
        if (token[i] == '$' && field[i] != 2)
        {
            value = check_var(&token[i+1], env, &i);
            if (value)
                write_value(ex_token, value, field, i);
            else
                skip_var(token, &i);
        }
        else
            ex_token->new_token[ex_token->index++] = token[i]; 
    }
    ex_token->new_token[ex_token->index] = '\0';
    return (ex_token->new_token);
}

void expansion(t_token *arg_list, t_env *env)
{
    int *field;

    if (!arg_list || !env)
        return;
    while (arg_list)
    {
        field = set_field(arg_list->value);
		arg_list->value = expanded_token(arg_list->value, field, env);
        arg_list = arg_list->next;
    }
}
