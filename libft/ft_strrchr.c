/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 21:32:35 by zfarouk           #+#    #+#             */
/*   Updated: 2024/11/06 21:54:21 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	j;
	int		len;

	j = (char)c;
	len = ft_strlen(s);
	while (len >= 0)
	{
		if (s[len--] == j)
			return ((char *)&s[++len]);
	}
	return (NULL);
}
