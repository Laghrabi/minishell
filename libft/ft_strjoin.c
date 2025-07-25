/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 20:41:06 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/12 21:15:29 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	j;
	size_t	i;
	char	*t;

	t = (char *)gc_malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (!t)
		return (NULL);
	i = 0;
	j = 0;
	if (s1)
		while (s1[i])
			t[j++] = s1[i++];
	i = 0;
	if (s2)
		while (s2[i])
			t[j++] = s2[i++];
	t[j] = 0;
	return (t);
}
