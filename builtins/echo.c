/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:47:08 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/04 23:21:26 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_echo(t_token *token, t_env *env_list)
{
	int	(i), (nl);
	nl = 0;
	if (token == NULL || env_list == NULL)
		return (1);
	token = token->next;
	if (token != NULL && ft_strcmp(token->value, "-n") == 0)
	{
		token = token->next;
		nl = 1;
	}
	while (token != NULL)
	{
		i = 0;
		while (token->value[i] != '\0')
		{
			if (write(1, &token->value[i], 1) == -1)
					return (1);
			i++;
		}
		if (token->next != NULL && write(1, " ", 1) == -1)
			return (1);
        if (token->next == NULL)
            update_env("_", token->value, env_list);
		token = token->next;
	}
	if (nl == 0 && write(1, "\n", 1) == -1)
		return (1);
	return (0);
}
