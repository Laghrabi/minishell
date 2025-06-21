/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   seperators_function.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 20:26:28 by zfarouk           #+#    #+#             */
/*   Updated: 2025/06/14 17:57:07 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"


t_token    *new_token(char *str, t_type type, int *field)
{
    t_token    *node;

    node = gc_malloc(sizeof(t_token));
    if (!node)
        return (NULL);
    node->value = ft_strdup(str);
    node->token = type;
    node->next = NULL;
    node->pre = NULL;
    node->ambiguous = 0;
    node->is_herdoc = 0;
    node->expansion = 1;
    node->field = field;
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

int operator_sep(char *input, t_info info, int *i, t_token **token)
{
    t_token *one_token;

    (void)input;
    one_token = new_token(info.str, info.type, NULL);
    add_back(token, one_token);
    *i = *i + info.size;
    return (1);
}

int operator(char c)
{
    if (c == '|'  || c == '<' || c == '>' || c == '(' || c == ')' || c == ' ')
        return (1);
    return (0);
}

void is_qoute(char c, int *i, int *j)
{
    if (c == '\'' && *j == 0 && *i == 0)
        *j = 1;
    else if(c == '\'' && *j == 1)
        *j = 0;
    else if (c == '\"' && *i == 0 && *j == 0)
        *i = 1;
    else if(c == '\"' && *i == 1)
        *i = 0;
}


int word_sep(char *input, t_info info, int *i, t_token **token)
{
    char *value;
    t_token *one_token;
    int (j), (db_qoute), (sg_qoute);

    db_qoute = 0;
    sg_qoute = 0;
    j = 0;
    while(input[j])
    {
        is_qoute(input[j], &db_qoute, &sg_qoute);
        if (db_qoute == 0 && sg_qoute == 0)
            if ((input[j] == '&' && input[j + 1] == '&') || operator(input[j]))
                break;
        j++;
    }
    if (db_qoute || sg_qoute)
        return (0);
    value = ft_substr(input, 0, j);
    one_token = new_token(value, info.type, NULL);
    one_token->token = info.type;
    add_back(token, one_token);
    *i = *i + j;
    return (1);
}
