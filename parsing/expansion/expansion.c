/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:04:58 by zfarouk           #+#    #+#             */
/*   Updated: 2025/06/11 17:43:23 by claghrab         ###   ########.fr       */
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
    else if (token[0] == '$')
    {
        *index = *index + 1;
        return (ft_strdup(""));
    }
    else 
    {
        *index = *index + 1;
        return (ft_strdup("$"));
    }
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

t_extoken *expanded_token(char *token, t_env *env)
{
    int (i), (db_quote), (sg_quote), (quote_state);
    t_extoken *ex_token;
    char *value;

    ex_token = gc_malloc(sizeof(t_extoken));
    ex_token->buffer_size = ft_strlen(token);
    ex_token->new_token = gc_malloc((ex_token->buffer_size + 1) * sizeof(char));
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
        ex_token->field[ex_token->index] = which_type(token[i], &db_quote, &sg_quote, &quote_state);
        ex_token->inherited_field = ex_token->field[ex_token->index];
        if (token[i] == '$' && sg_quote == 0)
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

void add_last(t_token **head, t_token *token)
{
    t_token *current;

    if (head == NULL || token == NULL)
        return;
    if (*head == NULL)
    {
        *head = token;
        return;
    }
    current = *head;
    while (current->next)
        current = current->next;
    current->next = token;
}


t_token *list_token(t_extoken *extoken)
{
    int i, word, l;
    char *one_token;
    t_token *token;
    t_token *head;
    char *value;
    
    head = NULL;
    value = extoken->new_token;
    i = 0;
    word = 0;
    l = 0;
    while (value[i])
    {
        if ((value[i] != ' ' && value[i] != '\t') && word == 0)
        {
            l = i;
            word = 1;
        }
        else if ((value[i] == ' ' || value[i] == '\t') && word == 1 && (extoken->field[i] == 1 || extoken->field[i] == 0))
        {
            one_token = ft_substr(value, l, i - l);
            token = new_token(one_token, T_WORD);
            add_last(&head, token);
            word = 0;
        }
        i++;
    }
    if (word == 1)
    {
        one_token = ft_substr(value, l, i - l);
        token = new_token(one_token, T_WORD);
        add_last(&head, token);
    }
    return (head);
}

void delete_node(t_token **arg_list)
{
    t_token *pre;
    t_token *next;
    
    pre = (*arg_list)->pre;
    next = (*arg_list)->next;
    if (pre)
        pre->next = next;
    if (next)
        next->pre = pre;
}

void split_expanded_token(t_extoken *extoken, t_token **arg_list)
{
    t_token *current;
    t_token *last;
    t_token *old = *arg_list;

    remove_extra_quote(extoken->new_token, extoken->field);
    current = list_token(extoken);
    if (!current)
    {
        delete_node(arg_list);
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

void expansion(t_token **arg_list, t_env *env)
{
    t_extoken *extoken;
    t_token *current;
    t_token *next;
    
    if (!arg_list || !(*arg_list) || !env)
        return;
    
    current = *arg_list;
    while (current)
    {
        extoken = expanded_token(current->value, env);
        printf("yes      %s\n", extoken->new_token);
        next = current->next;
        split_expanded_token(extoken, &current);
        // If current is the head of the list update arg_list to new head
        if (current->pre == NULL)
            *arg_list = current;
        current = next;
    }
}