/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 10:51:56 by zfarouk           #+#    #+#             */
/*   Updated: 2024/10/27 23:19:48 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	srclen;
	size_t	dstlen;
	size_t	j;

	srclen = ft_strlen((char *)src);
	if (dstsize == 0 || dstsize <= ft_strlen(dst))
		return (srclen + dstsize);
	dstlen = ft_strlen(dst);
	i = dstlen;
	j = 0;
	while ((j < dstsize - 1 - i) && src[j])
	{
		dst[i + j] = src[j];
		j++;
	}
	dst[i + j] = '\0';
	return (srclen + dstlen);
}
