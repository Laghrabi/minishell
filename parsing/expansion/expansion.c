/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:04:58 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/06 16:54:49 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int handle_single_quote(int *db_quote, int *sg_quote, int *quote_state)
{
    if (*db_quote == 1)
        return (*quote_state);
    else if (*sg_quote == 1)
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
    return (*quote_state);
}

int handle_double_quote(int *db_quote, int *sg_quote, int *quote_state)
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
    return (*quote_state);
}

int which_type(char c, int *db_quote, int *sg_quote, int *quote_state)
{
    if (c == '\'')
        return handle_single_quote(db_quote, sg_quote, quote_state);
    else if (c == '\"')
        return handle_double_quote(db_quote, sg_quote, quote_state);
    return (*quote_state);
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
    if (token[0] == '?')
    {
        char *exit_status = ft_itoa(s_var()->exit_status);
        *index += 1;
        return exit_status;
    }
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

void fill_extoken_fields(t_extoken *ex_token, char *token, t_env *env, int k)
{
    int i = -1;
    int db_quote = 0;
    int sg_quote = 0;
    int quote_state = 0;
    char *value;

    while (token[++i])
    {
        ex_token->field[ex_token->index] = which_type(token[i], &db_quote, &sg_quote, &quote_state) | k;
        ex_token->inherited_field = ex_token->field[ex_token->index];
        if (token[i] == '$' && (sg_quote == 0 || k != 0))
        {
            value = check_var(&token[i + 1], env, &i);
            if (value)
                write_value(ex_token, value);
            else
                skip_var(token, &i);
        }
        else
        {
            ex_token->new_token[ex_token->index++] = token[i];
        }
    }
}

t_extoken *expanded_token(char *token, t_env *env, int k)
{
    t_extoken *ex_token;

    if (!token)
        return (NULL);
    ex_token = gc_malloc(sizeof(t_extoken));
    ex_token->buffer_size = ft_strlen(token);
    ex_token->new_token = gc_malloc((ex_token->buffer_size + 1) * sizeof(char));
    ex_token->field = gc_malloc((ex_token->buffer_size + 1) * sizeof(int));
    ex_token->ambiguous = 0;
    ex_token->index = 0;
    ft_bzero(ex_token->new_token, ex_token->buffer_size + 1);
    ft_bzero(ex_token->field, ex_token->buffer_size + 1);
    fill_extoken_fields(ex_token, token, env, k);
    if (ex_token->index == 0 && ft_strchr(token, '$'))
        ex_token->ambiguous = 1;
    else if (ex_token->index == 0)
        ex_token->empty = 1;
    ex_token->new_token[ex_token->index] = '\0';
    return (ex_token);
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

void add_new_token(t_token **head, char *value, int *field, int start, int end)
{
    char *one_token = ft_substr(value, start, end - start);
    int *token_field = ft_subint(field, start, end - start);
    t_token *token = new_token(one_token, T_WORD, token_field);
    add_back(head, token);
}


t_token *list_token(t_extoken *extoken)
{
    int i = 0;
    int word = 0;
    int l = 0;
    t_token *head = NULL;
    char *value = extoken->new_token;
    int *field = extoken->field;

    while (value[i])
    {
        if (((value[i] != ' ' && value[i] != '\t') || 
            ((value[i] == ' ' || value[i] == '\t') && field[i] >= 2)) && word == 0)
        {
            l = i;
            word = 1;
        }
        else if ((value[i] == ' ' || value[i] == '\t') && word == 1 && (field[i] == 1 || field[i] == 0))
        {
            add_new_token(&head, value, field, l, i);
            word = 0;
        }
        i++;
    }
    if (word == 1)
        add_new_token(&head, value, field, l, i);
    return head;
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

void remove_extra_quote(char *token, int *field)
{
    int i = 0;
    int j = 0;
    int in_squote = 0;
    int in_dquote = 0;

    while (token[i])
    {
        if (token[i] == '\'' && !in_dquote && field[i] == 0)
        {
            in_squote = !in_squote;
            i++;
            continue;
        }
        else if (token[i] == '"' && !in_squote && field[i] == 0)
        {
            in_dquote = !in_dquote;
            i++;
            continue;
        }
        token[j] = token[i];
        field[j++] = field[i++];
    }
    token[j] = '\0';
}

void set_ambigouse(t_token *current)
{
    if (current->next)
        current->ambiguous = 1;
}

void split_expanded_token(t_extoken *extoken, t_token **arg_list, t_token **original_arg, int herdoc)
{
    t_token *current;
    t_token *last;
    t_token *old;

    if (!extoken)
        return ;
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
        set_ambigouse(current);
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
            if (extoken  && (extoken->ambiguous == 1 || extoken->empty == 1))
            {
                current->value = extoken->new_token;
                current->ambiguous = extoken->ambiguous;
                current->empty = extoken->empty;
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
