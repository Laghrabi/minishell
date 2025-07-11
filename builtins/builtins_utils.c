/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:20:21 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/11 10:46:53 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	replace_variable(int *flag, t_env *env_list, t_token *token)
{
	if (env_list == NULL)
		return ;
	if (*flag == 0)
	{
		if (token == NULL)
			update_env("_", "cd", env_list);
		else
			update_env("_", get_env_value("HOME", env_list), env_list);
	}
	else if (*flag == 1)
		update_env("_", "-", env_list);
	else
	{
		if (token->value[0] == '\0')
			update_env("_", "", env_list);
		else
			update_env("_", token->value, env_list);
	}
	return ;
}

int	cmd_or_builtin(t_token *token, t_env *env_list, char **argv)
{
	int flag;
	
	if (token == NULL || argv == NULL || *argv == NULL)
		return (0);
	flag = if_builtin(token->value);
	if (flag == 0)
		return(execute_simple_cmd(env_list, argv));
	else
		return(which_one(flag, token, env_list));
	return (-1);
}

int	which_one(int flag, t_token *token, t_env *env_list)
{
	if (flag == 1)
		return(builtin_cd(token, env_list));
	else if (flag == 2)
		return(builtin_echo(token, env_list));
	else if (flag == 3)
		return(builtin_pwd(token, env_list));
	else if (flag == 4)
		return(builtin_export(token, &env_list));
	else if (flag == 5)
		return(builtin_unset(token, env_list));
	else if (flag == 6)
		return(builtin_env(token, env_list));
	else if (flag == 7)
		return(builtin_exit(token));
	return (-1);
}

int	if_builtin(char *cmd)
{
	if (cmd == NULL)
		return (0);
	if (ft_strcmp(cmd, "cd") == 0)
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
