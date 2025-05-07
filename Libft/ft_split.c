/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 16:58:18 by claghrab          #+#    #+#             */
/*   Updated: 2024/11/02 17:54:46 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count(char const *s, char c)
{
	size_t	i;
	size_t	count;
	int		word;

	i = 0;
	word = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] == c)
			word = 0;
		if (s[i] != c && word == 0)
		{
			count++;
			word = 1;
		}
		i++;
	}
	return (count);
}

static char	**free_array(char **ptr, size_t i)
{
	while (i > 0)
	{
		i--;
		free(ptr[i]);
	}
	free(ptr);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;

	int (flag1), (flag2), (i), (j);
	flag1 = 0;
	flag2 = 0;
	if (!s)
		return (NULL);
	arr = ft_calloc(sizeof(char *), (count(s, c) + 1));
	if (!arr)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] == c && s[i])
			flag1 = ++i;
		while (s[i] && s[i] != c)
			flag2 = ++i;
		if (flag2 < flag1)
			break ;
		arr[j++] = ft_substr(s, flag1, (size_t)(flag2 - flag1));
		if (!arr[j - 1])
			return (free_array(arr, j));
	}
	return (arr);
}
