/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 14:16:51 by claghrab          #+#    #+#             */
/*   Updated: 2025/05/26 15:52:35 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cmd_or_builtin(t_token *token)
{
	int flag;
	
	if (token == NULL)
		return ;
	flag = if_builtin(token->value);
	if (if_builtin(token->value) == 0)
		exec_cmd();
	else
		which_one(flag, token);
}

void	which_one(int flag, t_token *token)
{
	if (flag == 1)
		builtin_cd(token);
	else if (flag == 2)
		builtin_echo(token);
	else if (flag == 3)
		builtin_pwd(token);
	else if (flag == 4)
		builtin_export(token);
	else if (flag == 5)
		builtin_unset(token);
	else if (flag == 6)
		builtin_env(token);
	else if (flag == 7)
		builtin_exit(token);
}

int	if_builtin(char *cmd)
{
	if (cmd == NULL)
		return (0);
	if (ft_cmp(cmd, "cd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "echo") == 0)
		return (2);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (3);
	else if (ft_strcmp(cmd, "export") ==0)
		return (4);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (5);
	else if (ft_strcmp(cmd, "env") == 0)
		return (6);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (7);
	else
		return (0);
}

int	builtin_echo(t_token *token)
{
	int	(i), (nl);
	nl = 0;
	if (token == NULL)
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
			if (token->value[i] == '\"' && token->value[i + 1] == '$')
				expansion();
			if (token->value[i] != '\'' && token->value[i] != '\"')
				if (write(1, &token->value[i], 1) == -1)
					return (1);
			i++;
		}
		if (token->next != NULL && write(1, " ", 1) == -1)
			return (1);
		token = token->next;
	}
	if (nl == 0 && write(1, "\n", 1) == -1)
		return (1);
	return (0);
}
