/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:04:58 by zfarouk           #+#    #+#             */
/*   Updated: 2025/05/31 14:35:45 by zfarouk          ###   ########.fr       */
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

// int  write_value(char **token, char *value, int *field, db_int yes)
// {
//     int len;
//     int i;
//     char *new_token;
    
//     if (field[yes.i] == 0)
//         fix_value(value);
//     len = ft_strlen(value);
//     new_token = gc_malloc((yes.len + len + 1) * sizeof(char));
//     len = 0;
//     while (len <= yes.i)
//     {
//         new_token[len] = (*token)[len];
//         len++;
//     }
//     i = -1;
//     printf("\n%s\n", new_token);
//     printf("%s", value);
//     while (value[++i])
//         new_token[len + i] = value[i];
//     new_token[len + i] = '\0';
//     printf("\n%s\n", new_token);
//     *token = new_token;
//     return (len + i);
// }

void write_value(t_extoken *ex_token, char *value, int *field)
{
    int i;
    int jl
    char *new_token;
    
    if (field[ex_token->index] == 4)
        fix_value(value);
    i = ft_strlen(value);
    new_token = gc_malloc((ex_token->buffer_size - ex_token->removed_quote + i + 1) * sizeof(char));
    ex_token->removed_quote = 0;
    i = -1;
    while (++i < ex_token->index)
        new_token[i] = ex_token->new_token[i];
    j = 0;
    while(value[j])
    {
        
    }
}

char *expanded_token(char *token, int *field, t_env *env)
{
    int (i), (len);
    t_extoken *ex_token;
    char *value;

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
            value = check_var(&token[i+1], env);
            if (value)
                write_value(ex_token, value, field);
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
