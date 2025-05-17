/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   seperators_function.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 20:26:28 by zfarouk           #+#    #+#             */
/*   Updated: 2025/05/16 17:53:21 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token    *new_token(char *str, t_type type)
{
    t_token    *node;

    node = malloc(sizeof(t_token));
    if (!node)
        return (NULL);
    node->value = ft_strdup(str);
    node->token = type;
    node->next = NULL;
    node->pre = NULL;
    return (node);
}


t_token    *last_token(t_token *lst)
{
    if (lst == NULL)
        return (NULL);
    while (lst->next != NULL)
        lst = lst->next;
    return (lst);
}


void    add_back(t_token **token, t_token *new)
{
    t_token    *current;

    if (token == NULL || new == NULL)
        return ;
    if (*token == NULL)
    {
        *token = new;
        return ;
    }
    current = last_token(*token);
    current->next = new;
    new->pre = current;
}

void operator_sep(char *input, t_info info, int *i, t_token **token)
{
    t_token *one_token;

    (void)input;
    one_token = new_token(info.str, info.type);
    add_back(token, one_token);
    *i = *i + info.size;
}

int operator(char c)
{
    if (c == '|'  || c == '<' || c == '>' || c == '(' ||c == ')')
        return (1);
    return (0);
}

void word_sep(char *input, t_info info, int *i, t_token **token)
{
    int j;
    char *value;
    t_token *one_token;

    j = 0;
    while(input[j] && input[j] != ' ')
    {
        if ((input[j] == '&' && input[j + 1] == '&') || operator(input[j]))
            break;
        j++;
    }
    value = ft_substr(input, 0, j);
    one_token = new_token(value, info.type);
    one_token->token = info.type;
    free (value);
    add_back(token, one_token);
    *i = *i + j;
}
