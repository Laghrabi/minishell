/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_outils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 20:02:07 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/08 21:53:56 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*list_token(t_extoken *extoken, int i, int l, int word)
{
	t_token	*head;
	char	*value;
	int		*field;

	head = NULL;
	value = extoken->new_token;
	field = extoken->field;
	while (value[i])
	{
		if ((!i_o(value[i]) || (i_o(value[i]) && field[i] >= 2)) && word == 0)
		{
			l = i;
			word = 1;
		}
		else if (i_o(value[i]) && word == 1 && (field[i] == 1 || field[i] == 0))
		{
			add_new_token(&head, value, field, (db_int){l, i});
			word = 0;
		}
		i++;
	}
	if (word == 1)
		add_new_token(&head, value, field, (db_int){l, i});
	return (head);
}

void	delete_node(t_token **arg_list, t_token **original_arg)
{
	t_token	*pre;
	t_token	*next;

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

void	remove_extra_quote(char *token, int *field)
{
	int (i), (j), (in_squote), (in_dquote);
	i = 0;
	j = 0;
	in_squote = 0;
	in_dquote = 0;
	while (token[i])
	{
		if (token[i] == '\'' && !in_dquote && field[i] == 0)
		{
			in_squote = !in_squote;
			i++;
			continue ;
		}
		else if (token[i] == '"' && !in_squote && field[i] == 0)
		{
			in_dquote = !in_dquote;
			i++;
			continue ;
		}
		token[j] = token[i];
		field[j++] = field[i++];
	}
	token[j] = '\0';
}

void	set_ambigouse(t_token *current)
{
	if (current->next)
    {
		current->ambiguous = 1;
        current->empty = 1;
    }
}

void	insert_expanded_tokens(t_token *current, t_token **arg_list)
{
	t_token	*old;
	t_token	*last;

	old = *arg_list;
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
