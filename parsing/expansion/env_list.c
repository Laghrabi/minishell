/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:10:56 by claghrab          #+#    #+#             */
/*   Updated: 2025/06/27 02:41:32 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	find_chr_pos(char *str, char c)
{
	int	i;
	
	if (str == NULL)
		return (-1);
	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

t_env	*find_last(t_env *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	env_add_back(t_env **lst, t_env *new)
{
	t_env	*current;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	current = find_last(*lst);
	current->next = new;
}

t_env	*init_env(char **envp)
{
	int		i;
	int		pos;
	t_env	*head;
	t_env	*node;
	
	if (envp == NULL || *envp == NULL)
		return (NULL);
	i = 0;
	head = NULL;
	while (envp[i] != NULL)
	{
		node = gc_malloc(sizeof(t_env));
		pos = find_chr_pos(envp[i], '=');
		node->key = ft_substr(envp[i], 0, pos);
		node->value = ft_substr(envp[i], pos + 1, ft_strlen(envp[i]) - pos - 1);
		node->next = NULL;
		env_add_back(&head, node);
		i++;
	}
	return (head);
}
