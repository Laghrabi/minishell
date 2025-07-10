/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:48:32 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/10 17:09:07 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void	mini_free(t_env *tmp)
{
	free(tmp->key);
	free(tmp->value);
	free(tmp);
}

void	remove_env_var(t_token *token)
{
	t_env (*current), (*tmp);
	current	= (*s_var()->env_list);
	while (current != NULL)
	{
		if (ft_strcmp(token->value, current->key) == 0)
		{
			tmp = current;
			if (current->prev == NULL)
			{
				(*s_var()->env_list) = current->next;
				if (current->next)
					current->next->prev = NULL;	
			}
			else
			{
				current->prev->next = current->next;
				if(current->next)
					current->next->prev = current->prev;
			}
			mini_free(tmp);
			break ;
		}
		current = current->next;
	}
}

int	builtin_unset(t_token *token, t_env *env_list)
{
	if (token == NULL || env_list == NULL)
		return (1);
	token = token->next;
	while (token != NULL)
	{
		remove_env_var(token);
		if (token->next == NULL)
			update_env("_", token->value, (*s_var()->env_list));
		token = token->next;
	}
	return (0);
}

// int	builtin_unset(t_token *token, t_env *env_list)
// {
// 	t_env   (*current), (*tmp);
// 	if (token == NULL || env_list == NULL)
// 		return (1);
// 	token = token->next;
// 	while (token != NULL)
// 	{
// 		current	= (*s_var()->env_list);
// 		while (current != NULL)
// 		{
// 			if (ft_strcmp(token->value, current->key) == 0)
// 			{
// 				tmp = current;
// 				if (current->prev == NULL)
// 				{
// 					(*s_var()->env_list) = current->next;
// 					if (current->next)
//     					current->next->prev = NULL;	
// 				}
// 				else
// 				{
//                 	current->prev->next = current->next;
// 					if(current->next)
// 						current->next->prev = current->prev;
// 				}
//                 free(tmp->key);
//                 free(tmp->value);
//                 free(tmp);
// 				break ;
// 			}
// 			current = current->next;
// 		}
// 		if (token->next == NULL)
// 			update_env("_", token->value, (*s_var()->env_list));
// 		token = token->next;
// 	}
// 	return (0);
// }
