/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unlink.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 17:11:32 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/12 22:38:46 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_file_name(t_ast *redir_node)
{
	if (redir_node == NULL)
		return (NULL);
	if (redir_node->token_list != NULL)
	{
		if (redir_node->token_list->ambiguous == 1)
		{
			ft_putstr_fd("minishell: ambiguous redirect\n", 2);
			return (NULL);
		}
		return (redir_node->token_list->value);
	}
	return (NULL);
}

t_unlinked	*new_file(void *ptr)
{
	t_unlinked	*node;

	node = gc_malloc(sizeof(t_unlinked));
	if (!node)
		return (NULL);
	node->file_name = ptr;
	node->next = NULL;
	return (node);
}

void	unlink_func(char *path_name, int i)
{
	static t_unlinked	*head;
	static t_unlinked	*current;
	t_unlinked			*new;

	if (path_name)
	{
		new = new_file(path_name);
		if (!new)
			return ;
		if (!current)
			head = new;
		else
			current->next = new;
	}
	if (i == 1)
	{
		while (head)
		{
			unlink(head->file_name);
			head = head->next;
		}
		head = NULL;
		current = NULL;
	}
}
