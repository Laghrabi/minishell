/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 14:16:51 by claghrab          #+#    #+#             */
/*   Updated: 2025/05/25 18:32:32 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cmd_or_builtin(t_token *token)
{
	int flag;
	
	if (token == NULL)
		return ;
	flag = if_builtin(token->value);
	while (token != NULL)
	{
		if (if_builtin(token->value) == 0)
			exec_cmd();
		else
			which_one(flag, token);
	}
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
	if (ft_strncmp(cmd, "cd", 2) == 0)
		return (1);
	else if (ft_strncmp(cmd, "echo", 4) == 0)
		return (2);
	else if (ft_strncmp(cmd, "pwd", 3) == 0)
		return (3);
	else if (ft_strncmp(cmd, "export", 6) == 0)
		return (4);
	else if (ft_strncmp(cmd, "unset", 5) == 0)
		return (5);
	else if (ft_strncmp(cmd, "env", 3) == 0)
		return (6);
	else if (ft_strncmp(cmd, "exit", 4) == 0)
		return (7);
	else
		return (0);
}

int	builtin_echo(t_token *token)
{
	int	i;
	int nl;
	int	exit_status;

	nl = 0;
	i = 0;
	if (token == NULL);
		return ;
	if (ft_strncmp(token->next->value, "-n", ft_strlen(token->next->value)) == 0)
	{
		token = token->next;
		nl = 1;
	}
	token = token->next;
	while (token != NULL)
	{
		while (token->value[i] != '\0')
		{
			if (token->value[i] == '\"' && token->value[i + 1] == '$')
				expansion();
			if (token->value[i] != '\'' && token->value[i] != '\"')
			exit_status = write(1, &token->value[i++], 1);
			if (exit_status == -1)
			return (1);
		}
		token = token->next;
		if (token == NULL && nl == 1)
			write(1, '\n', 1);
	}
}
