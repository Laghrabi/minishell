/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 23:15:48 by zfarouk           #+#    #+#             */
/*   Updated: 2024/11/06 17:17:28 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static int	info_num(int n, int *sign)
{
	int	i;

	*sign = 1;
	i = 0;
	if (n < 0)
	{
		i++;
		*sign = -1;
	}
	while (n != 0)
	{
		i++;
		n /= 10;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	int		i;
	char	*p;
	int		sign;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n == 0)
		return (ft_strdup("0"));
	i = info_num(n, &sign);
	if (n < 0)
		n *= -1;
	p = malloc(i + 1);
	if (!p)
		return (NULL);
	p[i] = '\0';
	while (n != 0)
	{
		p[--i] = ((n % 10) + 48);
		n /= 10;
	}
	if (sign == -1)
		p[--i] = '-';
	return (p);
}
