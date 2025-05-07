/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:30:14 by claghrab          #+#    #+#             */
/*   Updated: 2024/11/08 00:18:57 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	to_start(const char *str, const char *set)
{
	size_t	x;
	size_t	y;

	x = 0;
	y = 0;
	while (str[x] && set[y])
	{
		y = 0;
		while (set[y])
		{
			if (str[x] == set[y])
			{
				x++;
				break ;
			}
			y++;
		}
	}
	return (x);
}

static int	to_end(const char *str, const char *set)
{
	size_t	x;
	size_t	y;

	x = ft_strlen((char *)str) - 1;
	y = 0;
	while (x > 0 && set[y])
	{
		y = 0;
		while (set[y])
		{
			if (str[x] == set[y])
			{
				x--;
				break ;
			}
			y++;
		}
	}
	return (x);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	x;
	size_t	y;
	char	*array;

	if (!s1 || !set)
		return (NULL);
	x = to_start(s1, set);
	if (ft_strlen(s1) == 0 || x == ft_strlen(s1))
		return (ft_strdup(""));
	y = to_end(s1, set);
	array = malloc(sizeof(char) * (y - x + 2));
	if (!array)
		return (NULL);
	i = 0;
	while (y >= x)
		array[i++] = s1[x++];
	array[i] = '\0';
	return (array);
}
