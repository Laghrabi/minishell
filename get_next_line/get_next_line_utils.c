/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 14:06:44 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/12 21:11:17 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	append(t_listg **lst, t_listg *node)
{
	t_listg	*current;

	if (*lst == NULL)
	{
		*lst = node;
		return ;
	}
	current = *lst;
	while (current->next)
		current = current->next;
	current->next = node;
}

void	make(int fd, t_listg **the_line)
{
	t_listg	*node;

	while (checkk(*the_line))
	{
		node = new_node10(fd, the_line);
		if (!node)
			return ;
		append(the_line, node);
	}
}

t_listg	*new_node10(int fd, t_listg **the_line)
{
	t_listg	*lst;
	char	*buffer;
	int		i;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
	{
		clear(the_line);
		return (NULL);
	}
	i = read(fd, buffer, BUFFER_SIZE);
	if (i <= 0)
	{
		free(buffer);
		if (i == -1)
			clear(the_line);
		return (NULL);
	}
	buffer[i] = '\0';
	lst = malloc(sizeof(t_listg));
	if (!lst)
		return (free(buffer), NULL);
	lst->word = buffer;
	lst->next = NULL;
	return (lst);
}

int	checkk(t_listg *node)
{
	int	i;

	if (node == NULL || node->word == NULL)
		return (1);
	while (node)
	{
		i = 0;
		while (node->word[i] && i < BUFFER_SIZE)
		{
			if (node->word[i] == '\n')
				return (0);
			i++;
		}
		node = node->next;
	}
	return (1);
}
