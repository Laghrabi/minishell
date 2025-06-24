/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:04:58 by zfarouk           #+#    #+#             */
/*   Updated: 2025/06/24 18:55:14 by zfarouk          ###   ########.fr       */
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

void skip_var(char *token, int *i)
{
    (*i)++;
    while (ft_isalnum(token[*i]) || token[*i] == '_')
        (*i)++;
    (*i)--;
}

char *check_var(char *token, t_env *env, int *index)
{
    size_t i;
    char *var_name;
    char *value;

    i = 0;
    /* should handle $?   
        if (token[0] == '?')
        {
            char *exit_status = ft_itoa(...)
            *index += 1;
            return exit_status;
        }
    */
    value = NULL;
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
        return (ft_strdup("$"));
    return (ft_strdup(value));
}

void write_value(t_extoken *ex_token, char *value)
{
    int i;
    int j;
    char *new_token;
    int *new_field;
    
    ex_token->buffer_size += ft_strlen(value);
    new_token = gc_malloc((ex_token->buffer_size + 1) * sizeof(char));
    new_field = gc_malloc((ex_token->buffer_size + 1) * sizeof(int));
    ft_bzero(new_field, (ex_token->buffer_size + 1) * sizeof(int));
    i = -1;
    while (++i < ex_token->index)
    {
        new_token[i] = ex_token->new_token[i];
        new_field[i] = ex_token->field[i];
    }
    j = 0;
    while(value[j])
    {
        new_token[i + j] = value[j];
        new_field[i + j] = ex_token->inherited_field | 1;
        j++;
    }
    ex_token->index = j + i;
    ex_token->new_token = new_token;
    ex_token->field = new_field;
}

t_extoken *expanded_token(char *token, t_env *env, int k)
{
    int (i), (db_quote), (sg_quote), (quote_state);
    t_extoken *ex_token;
    char *value;

    if (!token)
        return (NULL);
    ex_token = gc_malloc(sizeof(t_extoken));
    ex_token->buffer_size = ft_strlen(token);
    ex_token->new_token = gc_malloc((ex_token->buffer_size + 1) * sizeof(char));
    ex_token->ambiguous = 0;
    ft_bzero(ex_token->new_token, ex_token->buffer_size + 1);
    ex_token->field = gc_malloc((ex_token->buffer_size + 1) * sizeof(int));
    ft_bzero(ex_token->field, (ex_token->buffer_size + 1) * sizeof(int));    
    ex_token->index = 0;
    i = -1;
    db_quote = 0;     
    sg_quote = 0;
    quote_state = 0;
    while (token[++i])
    {
        ex_token->field[ex_token->index] = which_type(token[i], &db_quote, &sg_quote, &quote_state) | k;
        ex_token->inherited_field = ex_token->field[ex_token->index];
        if (token[i] == '$' && (sg_quote == 0 || k != 0))
        {
            value = check_var(&token[i+1], env, &i);
            if (value)
                write_value(ex_token, value);
            else
                skip_var(token, &i);
        }
        else
        ex_token->new_token[ex_token->index++] = token[i];
    }
    if (ex_token->index == 0)
        ex_token->ambiguous = 1;
    ex_token->new_token[ex_token->index] = '\0';
    return (ex_token);
}

void remove_extra_quote(char *token, int *field)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (token[i])
    {
        if ((token[i] == '\'' || token[i] == '\"') && field[i] == 0 )
        {
            j = i;
            while (token[j])
            {
                token[j] = token[j + 1];
                field[j] = field[j + 1];
                j++;
            }
            continue;
        }
        i++;
    }
}

int *ft_subint(int *field, int start, int len)
{
    int i;
    int *new_field;

    if (!field)
        return (NULL);
    new_field = gc_malloc(len * sizeof(int));
    i = 0;
    while (i < len)
    {
        new_field[i] = field[start + i];
        i++;
    }
    return (new_field);
}

t_token * list_token(t_extoken *extoken)
{
    int i, word, l;
    char *one_token;
    t_token *token;
    t_token *head;
    char *value;
    int *token_field;
    
    head = NULL;
    token_field = NULL;
    value = extoken->new_token;
    i = 0;
    word = 0;
    l = 0;
    while (value[i])
    {
        if (((value[i] != ' ' && value[i] != '\t') || ((value[i] == ' ' || value[i] == '\t') && extoken->field[i] >= 2)) && word == 0)
        {
            l = i;
            word = 1;
        }
        else if ((value[i] == ' ' || value[i] == '\t') && word == 1 && (extoken->field[i] == 1 || extoken->field[i] == 0))
        {
            one_token = ft_substr(value, l, i - l);
            token_field = ft_subint(extoken->field, l, i - l);
            token = new_token(one_token, T_WORD, token_field);
            add_back(&head, token);
            word = 0;
        }
        i++;
    }
    if (word == 1)
    {
        one_token = ft_substr(value, l, i - l);
        token_field = ft_subint(extoken->field, l, i - l);
        token = new_token(one_token, T_WORD, token_field);
        add_back(&head, token);
    }
    return (head);
}

void delete_node(t_token **arg_list, t_token **original_arg)
{
    t_token *pre;
    t_token *next;
    
    pre = (*arg_list)->pre;
    next = (*arg_list)->next;
    if (pre)
        pre->next = next;
    if (next)
        next->pre = pre;
    if (*arg_list == *original_arg && (*original_arg)->pre == NULL)
        *original_arg = (*original_arg)->next;
    *arg_list = next;
}

void split_expanded_token(t_extoken *extoken, t_token **arg_list, t_token **original_arg, int herdoc)
{
    t_token *current;
    t_token *last;
    t_token *old;

    (void)original_arg;
    (*arg_list)->field = extoken->field;
    old = *arg_list;
    remove_extra_quote(extoken->new_token, extoken->field);
    if (herdoc)
        (*arg_list)->value = extoken->new_token;
    else
    {
        current = list_token(extoken);
        if (!current)
        {
            (*arg_list)->value = ft_strdup("");
            return;
        }
        last = current;
        while (last->next)
            last = last->next;
        if (old->pre)
        {
            old->pre->next = current;
            current->pre = old->pre;
        }
        else
            current->pre = NULL;
        if (old->next)
        {
            old->next->pre = last;
            last->next = old->next;
        }
        else
            last->next = NULL;
        if (old->pre == NULL)
            *arg_list = current;
    }
}

void expansion(t_token **arg_list, t_env *env)
{
    t_extoken (*extoken);
    t_token (*current), (*next);
    extoken = NULL;
    if (!arg_list || !(*arg_list) || !env)
        return;
    current = *arg_list;
    while (current)
    {
        next = current->next;
        if (current->expansion)
        {
            extoken = expanded_token(current->value, env, current->is_herdoc);
            if (extoken->ambiguous == 1)
            {
                current->value = extoken->new_token;
                current->ambiguous = 1;
                current->empty = 1;
                current = next;
                continue;
            }
            split_expanded_token(extoken, &current, arg_list, current->is_herdoc);
            if (current && current->pre == NULL)
                *arg_list = current;
        }
        current = next;
    }
}
