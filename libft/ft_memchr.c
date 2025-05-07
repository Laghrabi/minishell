/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 11:24:40 by zfarouk           #+#    #+#             */
/*   Updated: 2024/11/06 17:06:31 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*k;
	size_t			i;
	unsigned char	j;

	j = (unsigned char)c;
	k = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		if (k[i] == j)
			return (&k[i]);
		i++;
	}
	return (NULL);
}
