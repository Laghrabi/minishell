/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:48:10 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/04 22:50:08 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_env(t_token *token, t_env *env_list)
{
	t_env *current;

	if (token == NULL || env_list == NULL)
		return (1);
	if (token->next != NULL)
	{
		printf("env: too many arguments\n");
		return (1);
	}
	current = env_list;
	while (current != NULL)
	{
		if (current->value != NULL) //&& current->value[0] != '\0')
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	return (0);
}
