/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:47:08 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/06 00:07:55 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int is_n_flag(char *flag)
{
	int i;

	i = 1;
	if (flag[0] != '-')
		return (1);
	while (flag[i])
	{
		if (flag[i] != 'n')
			return (1);
		i++;
	}
	return (0);
}

int	builtin_echo(t_token *token, t_env *env_list)
{
	int	(i), (nl);
	nl = 0;
	if (token == NULL || env_list == NULL)
		return (1);
	token = token->next;
	if (token != NULL && is_n_flag(token->value) == 0)
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
		if (token->next != NULL && !token->ambiguous)
			write(1, " ", 1);
        if (token->next == NULL)
            update_env("_", token->value, env_list);
		token = token->next;
	}
	if (nl == 0 && write(1, "\n", 1) == -1)
		return (1);
	return (0);
}
