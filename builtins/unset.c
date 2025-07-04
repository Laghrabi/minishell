/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:48:32 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/04 22:54:54 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_unset(t_token *token, t_env **env_list)
{
	t_env   (*current), (*tmp);
	if (token == NULL || env_list == NULL  || *env_list == NULL)
		return (1);
	token = token->next;
	while (token != NULL && ft_strcmp((*env_list)->key, token->value) == 0)
	{
		*env_list = (*env_list)->next;
		token = token->next;	
	}
	while (token != NULL)
	{
		current	= *env_list;
		while (current != NULL)
		{
			if (current->next != NULL && ft_strcmp(token->value, current->next->key) == 0)
			{
                tmp = current->next;
				current->next = current->next->next;
                free(tmp->key);
                free(tmp->value);
                free(tmp);
				break ;
			}
			current = current->next;
		}
		token = token->next;
	}
	return (0);
}
