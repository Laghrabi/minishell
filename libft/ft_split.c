/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 22:31:16 by zfarouk           #+#    #+#             */
/*   Updated: 2024/11/06 21:52:14 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_word(char const *s, char c, int *size)
{
	size_t	i;
	size_t	word;
	int		count;

	i = 0;
	word = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] != c && word == 0)
		{
			word = 1;
			count++;
		}
		if (s[i] == c && word == 1)
			word = 0;
		i++;
	}
	*size = i;
	return (count);
}

static char	**free_array(char **ptr, int i)
{
	while (i > 0)
	{
		i--;
		free(ptr[i]);
	}
	free(ptr);
	return (NULL);
}

static char	**allocation(char **p, char const *s, char c, int size)
{
	int (j), (i), (word), (l);
	j = 0;
	i = 0;
	word = 0;
	l = 0;
	while (i <= size)
	{
		if ((s[i] != c && s[i] != 0) && word == 0)
		{
			l = i;
			word = 1;
		}
		if ((s[i] == c || s[i] == '\0') && word == 1)
		{
			p[j] = ft_substr(s, (unsigned int)l, (size_t)(i - l));
			if (!p[j])
				return (free_array(p, j));
			j++;
			word = 0;
		}
		i++;
	}
	return (p);
}

char	**ft_split(char const *s, char c)
{
	char	**p;
	int		i;
	int		size_s;

	if (!s)
		return (NULL);
	i = count_word(s, c, &size_s);
	p = malloc((i + 1) * sizeof(char *));
	if (!p)
		return (NULL);
	p = allocation(p, s, c, size_s);
	if (!p)
		return (NULL);
	p[i] = NULL;
	return (p);
}
