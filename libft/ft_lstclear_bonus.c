/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 18:15:19 by zfarouk           #+#    #+#             */
/*   Updated: 2024/11/10 18:17:12 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*current;
	t_list	*the_next;

	if (!del || !*lst || !lst)
		return ;
	current = *lst;
	while (current)
	{
		the_next = current->next;
		ft_lstdelone(current, del);
		current = the_next;
	}
	*lst = NULL;
}
