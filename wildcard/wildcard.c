/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 18:04:27 by zfarouk           #+#    #+#             */
/*   Updated: 2025/06/24 21:01:53 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_matche(char *str, char *suspect)
{
	if (*str == '\0')
		return (*suspect == '\0');
	if (*str == '*')
		return (is_matche(str + 1, suspect) || (*suspect != '\0'
				&& is_matche(str, suspect + 1)));
	if (*suspect != '\0' && *suspect == *str)
		return (is_matche(str + 1, suspect + 1));
	return (0);
}

t_token	*search_matches(char *str)
{
	DIR				*dir;
	struct dirent	*entry;
	t_token			*node;
	t_token			*head;

	head = NULL;
	dir = opendir(".");
	if (!dir)
		return (NULL);
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] == '.' && str[0] != '.')
		{
			entry = readdir(dir);
			continue ;
		}
		if (is_matche(str, entry->d_name))
		{
			node = new_token(entry->d_name, T_WORD, NULL);
			add_back(&head, node);
		}
		entry = readdir(dir);
	}
	return (head);
}

void	append_matches(t_token **token, t_token *match)
{
	t_token	*prev;
	t_token	*next;

	prev = (*token)->pre;
	next = (*token)->next;
	match->pre = prev;
	if (prev)
		prev->next = match;
	while (match->next)
		match = match->next;
	match->next = next;
	if (next)
		next->pre = match;
	*token = match;
}

void	check_wildcard(t_token **token)
{
	t_token	*matches;
	int		i;

	matches = NULL;
	if (!token)
		return ;
	i = 0;
	while ((*token)->value[i])
	{
		if ((*token)->value[i] == '*' && ((*token)->field[i] == 0
				|| (*token)->field[i] == 1))
		{
			matches = search_matches((*token)->value);
			if (matches)
				append_matches(token, matches);
			return ;
		}
		i++;
	}
}

void	wildcard(t_token **arg_list)
{
	int		check;
	t_token	*test;
	t_token	*current;

	check = 0;
	if (arg_list == NULL || *arg_list == NULL)
		return ;
	current = *arg_list;
	while (current)
	{
		if (current->is_herdoc == 0)
		{
			check_wildcard(&current);
			if (!check)
			{
				test = current;
				while (test->pre)
					test = test->pre;
				*arg_list = test;
			}
			if (current->pre == NULL)
				*arg_list = current;
		}
		current = current->next;
	}
}
