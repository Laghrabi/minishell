/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 15:12:06 by claghrab          #+#    #+#             */
/*   Updated: 2024/11/06 21:43:20 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	size_t	tot_size;
	void	*array;

	tot_size = count * size;
	if (count != 0 && (size > SIZE_MAX / count))
		return (NULL);
	array = malloc(tot_size);
	if (!array)
		return (NULL);
	ft_bzero(array, tot_size);
	return (array);
}
