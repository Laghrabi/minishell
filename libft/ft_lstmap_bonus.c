/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 12:47:30 by zfarouk           #+#    #+#             */
/*   Updated: 2024/11/11 14:09:12 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*reel;
	t_list	*test;
	void	*tmp;

	if (!lst || !del || !f)
		return (NULL);
	reel = NULL;
	while (lst)
	{
		tmp = f(lst->content);
		test = ft_lstnew(tmp);
		if (!test)
		{
			del(tmp);
			ft_lstclear(&reel, del);
			return (NULL);
		}
		else
			ft_lstadd_back(&reel, test);
		lst = lst->next;
	}
	return (reel);
}
