/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:41:55 by claghrab          #+#    #+#             */
/*   Updated: 2025/06/01 18:36:12 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int ft_strcmp(char *s1, char *s2)
{
	int	i;
    int j;

    if (!s1 || !s2)
        return (1);
i = 0;
    j = 0;
	while (s1[j]|| s2[i])
    {
        if (s1[j] != s2[i])
			return ((unsigned char)s1[j] - (unsigned char)s2[i]);
		i++;
        j++;
    }
	return (s1[i] - s2[i]);
}
