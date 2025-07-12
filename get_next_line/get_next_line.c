/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:50:27 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/12 20:21:22 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	size_lst(t_listg *lst)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (lst)
	{
		i = 0;
		while (lst->word && lst->word[i])
		{
			if (lst->word[i] == '\n')
			{
				len++;
				return (len);
			}
			i++;
			len++;
		}
		lst = lst->next;
	}
	return (len);
}

char	*squeezer(t_listg *lst)
{
	int		i;
	int		len;
	char	*buffer;

	len = size_lst(lst);
	buffer = malloc(len + 1);
	if (!buffer)
		return (NULL);
	len = 0;
	while (lst != NULL)
	{
		i = 0;
		while (lst->word && lst->word[i])
		{
			if (lst->word[i] == '\n')
			{
				buffer[len++] = '\n';
				buffer[len++] = '\0';
				return (buffer);
			}
			buffer[len++] = lst->word[i++];
		}
		lst = lst->next;
	}
	return (buffer[len++] = '\0', buffer);
}

void	clear(t_listg **lst)
{
	t_listg	*current;
	t_listg	*the_next;

	if (!*lst || !lst)
		return ;
	current = *lst;
	while (current)
	{
		the_next = current->next;
		free(current->word);
		free(current);
		current = the_next;
	}
	*lst = NULL;
}

char	*rest(t_listg *lst)
{
	int		len;
	int		i;
	int		j;
	char	*remains;

	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	i = 0;
	while (lst->word[i] && lst->word[i] != '\n')
		i++;
	if (!lst->word[i] || lst->word[i + 1] == '\0')
		return (NULL);
	j = 0;
	len = i + 1;
	while (lst->word[++i])
		j++;
	remains = malloc(j + 1);
	if (!remains)
		return (NULL);
	j = 0;
	while (lst->word[len])
		remains[j++] = lst->word[len++];
	return (remains[j] = '\0', remains);
}

char	*get_next_line(int fd)
{
	static t_listg	*the_line = NULL;
	char			*the_bottom_line;
	char			*remains;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	make(fd, &the_line);
	if (!the_line)
		return (NULL);
	the_bottom_line = squeezer(the_line);
	remains = rest(the_line);
	clear(&the_line);
	if (remains)
	{
		the_line = malloc(sizeof(t_listg));
		if (!the_line)
		{
			free(remains);
			return (NULL);
		}
		the_line->word = remains;
		the_line->next = NULL;
	}
	return (the_bottom_line);
}
