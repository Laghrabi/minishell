/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:47:08 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/11 10:47:10 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_n_flag(char *flag)
{
	int	i;

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

int	handle_echo_options(t_token **token, int *nl_flag)
{
	if (*token != NULL && is_n_flag((*token)->value) == 0)
	{
		*token = (*token)->next;
		*nl_flag = 1;
	}
	return (0);
}

int	print_echo_args(t_token *token, t_env *env_list, int nl_flag)
{
	int	i;

	while (token != NULL)
	{
		i = 0;
		while (token->value[i] != '\0')
		{
			if (write(1, &token->value[i], 1) == -1)
				return (1);
			i++;
		}
		if (token->next != NULL && (!token->ambiguous || token->empty))
			write(1, " ", 1);
		if (token->next == NULL)
			update_env("_", token->value, env_list);
		token = token->next;
	}
	if (nl_flag == 0 && write(1, "\n", 1) == -1)
		return (1);
	return (0);
}

int	builtin_echo(t_token *token, t_env *env_list)
{
	int	nl_flag;

	if (token == NULL)
		return (1);
	nl_flag = 0;
	token = token->next;
	handle_echo_options(&token, &nl_flag);
	return (print_echo_args(token, env_list, nl_flag));
}

// int	builtin_echo(t_token *token, t_env *env_list)
// {
// 	int	(i), (nl);
// 	nl = 0;
// 	if (token == NULL || env_list == NULL)
// 		return (1);
// 	token = token->next;
// 	if (token != NULL && is_n_flag(token->value) == 0)
// 	{
// 		token = token->next;
// 		nl = 1;
// 	}
// 	while (token != NULL)
// 	{
// 		i = 0;
// 		while (token->value[i] != '\0')
// 		{
// 			if (write(1, &token->value[i], 1) == -1)
// 					return (1);
// 			i++;
// 		}
// 		if (token->next != NULL && (!token->ambiguous || token->empty))
// 			write(1, " ", 1);
//         if (token->next == NULL)
//             update_env("_", token->value, env_list);
// 		token = token->next;
// 	}
// 	if (nl == 0 && write(1, "\n", 1) == -1)
// 		return (1);
// 	return (0);
// }
