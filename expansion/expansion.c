/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:04:58 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/12 22:28:55 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	split_expanded_token(t_extoken *extoken, t_token **arg_list,
		t_token **original_arg, int herdoc)
{
	t_token	*current;

	if (!extoken)
		return ;
	(void)original_arg;
	(*arg_list)->field = extoken->field;
	if (herdoc)
	{
		(*arg_list)->value = extoken->new_token;
		return ;
	}
	remove_extra_quote(extoken->new_token, extoken->field);
	current = list_token(extoken, 0, 0, 0);
	if (!current)
	{
		(*arg_list)->value = ft_strdup("");
		return ;
	}
	set_ambigouse(current);
	insert_expanded_tokens(current, arg_list);
}

void	fill_extoken_fields(t_extoken *ex_token, char *token, t_env *env, int k)
{
	char	*value;

	int (i), (db_quote), (sg_quote), (quote_state);
	i = -1;
	db_quote = 0;
	sg_quote = 0;
	quote_state = 0;
	while (token[++i])
	{
		ex_token->field[ex_token->index] = which_type(token[i], &db_quote,
				&sg_quote, &quote_state) | k;
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

t_extoken	*expanded_token(char *token, t_env *env, int k)
{
	t_extoken	*ex_token;

	if (!token)
		return (NULL);
	ex_token = gc_malloc(sizeof(t_extoken));
	ex_token->buffer_size = ft_strlen(token);
	ex_token->new_token = gc_malloc((ex_token->buffer_size + 1) * sizeof(char));
	ex_token->field = gc_malloc((ex_token->buffer_size + 1) * sizeof(int));
	ex_token->ambiguous = 0;
	ex_token->empty = 0;
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

void	process_token_expansion(t_token **current, t_token **next,
		t_token **arg_list, t_env *env)
{
	t_extoken	*extoken;

	(void) next;
	extoken = expanded_token((*current)->value, env, (*current)->is_herdoc);
	if (extoken && (extoken->ambiguous == 1 || extoken->empty == 1))
	{
		(*current)->value = extoken->new_token;
		(*current)->ambiguous = extoken->ambiguous;
		(*current)->empty = extoken->empty;
		return ;
	}
	split_expanded_token(extoken, current, arg_list, (*current)->is_herdoc);
	if (*current && (*current)->pre == NULL)
		*arg_list = *current;
}

void	expansion(t_token **arg_list, t_env *env)
{
	t_token	*current;
	t_token	*next;

	if (!arg_list || !(*arg_list) || !env)
		return ;
	current = *arg_list;
	while (current)
	{
		next = current->next;
		if (current->expansion)
			process_token_expansion(&current, &next, arg_list, env);
		current = next;
	}
}
// void expansion(t_token **arg_list, t_env *env)
// {
//     t_extoken (*extoken);
//     t_token (*current), (*next);
//     extoken = NULL;
//     if (!arg_list || !(*arg_list) || !env)
//         return ;
//     current = *arg_list;
//     while (current)
//     {
//         next = current->next;
//         if (current->expansion)
//         {
//             extoken = expanded_token(current->value, env,
//		current->is_herdoc);
//             if (extoken  && (extoken->ambiguous == 1 || extoken->empty == 1))
//             {
//                 current->value = extoken->new_token;
//                 current->ambiguous = extoken->ambiguous;
//                 current->empty = extoken->empty;
//                 current = next;
//                 continue ;
//             }
//             split_expanded_token(extoken, &current, arg_list,
//		current->is_herdoc);
//             if (current && current->pre == NULL)
//                 *arg_list = current;
//         }
//         current = next;
//     }
// }

// void split_expanded_token(t_extoken *extoken, t_token **arg_list,
//		t_token **original_arg, int herdoc)
// {
//     t_token *current;
//     t_token *last;
//     t_token *old;

//     if (!extoken)
//         return ;
//     (void)original_arg;
//     (*arg_list)->field = extoken->field;
//     old = *arg_list;
//     remove_extra_quote(extoken->new_token, extoken->field);
//     if (herdoc)
//         (*arg_list)->value = extoken->new_token;
//     else
//     {
//         current = list_token(extoken);
//         if (!current)
//         {
//             (*arg_list)->value = ft_strdup("");
//             return ;
//         }
//         set_ambigouse(current);
//         last = current;
//         while (last->next)
//             last = last->next;
//         if (old->pre)
//         {
//             old->pre->next = current;
//             current->pre = old->pre;
//         }
//         else
//             current->pre = NULL;
//         if (old->next)
//         {
//             old->next->pre = last;
//             last->next = old->next;
//         }
//         else
//             last->next = NULL;
//         if (old->pre == NULL)
//             *arg_list = current;
//     }
// char *check_var(char *token, t_env *env, int *index)
// {
//     size_t i;
//     char *var_name;
//     char *value;

//     i = 0;
//     if (token[0] == '?')
//     {
//         char *exit_status = ft_itoa(s_var()->exit_status);
//         *index += 1;
//         return (exit_status);
//     }
//     value = NULL;
//     if (ft_isalpha(token[0]) || token[0] == '_')
//     {
//         while (ft_isalnum(token[i]) || token[i] == '_')
//             i++;
//         var_name = ft_substr(token, 0, i);
//         value = get_env_value(var_name, env);
//         if (value)
//             *index = *index + ft_strlen(var_name);
//     }
//     else if (ft_isdigit(token[0]))
//     {
//         *index += 1;
//         return (ft_strdup(""));
//     }
//     else
//         return (ft_strdup("$"));
//     return (ft_strdup(value));
// }
// }