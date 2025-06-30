/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 09:51:36 by zfarouk           #+#    #+#             */
/*   Updated: 2025/06/30 16:45:57 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int				i;
	unsigned char	j;

	i = 0;
	j = (unsigned char)c;
	while (s[i])
	{
		if (s[i] == j)
			return ((char *)&s[i]);
		i++;
	}
	if (s[i] == j)
		return ((char *)&s[i]);
	return (NULL);
}