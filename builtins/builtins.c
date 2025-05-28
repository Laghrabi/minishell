/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 14:16:51 by claghrab          #+#    #+#             */
/*   Updated: 2025/05/28 16:14:56 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	cd_helper(char *new_path, t_env *env_list)
{
	char	*old_pwd;
	char	*new_pwd;
	
	if (new_path == NULL || env_list == NULL)
		return (1);
	old_pwd = getcwd(NULL, 0);
	if (chdir(new_path) == -1)
	{
		printf("cd: %s: No such file or directory\n", new_path);
		free(old_pwd);
		garbage_collector(NULL, 1);
		return (1);
	}
	new_pwd = getcwd(NULL, 0);
	update_env("OLDPWD", old_pwd, env_list);
	update_env("PWD", new_pwd, env_list);
	free(old_pwd);
	free(new_pwd);
	return (0);
}

int	builtin_cd(t_token *token, t_env *env_list)
{
	char	*new_path;
	
	if (token == NULL || env_list == NULL)
		return (1);
	if (token->next == NULL || ft_strcmp(token->next->value, "~") == 0)
		new_path = get_env_value("HOME", env_list);
	else if (ft_strcmp(token->next->value, "-") == 0)
		new_path = get_env_value("OLDPWD", env_list);
	else
		new_path = token->next->value;
	if (cd_helper(new_path, env_list) == 1)
		return (1);
	return (0);
}

int	builtin_pwd(t_token *token, t_env *env_list)
{
	char	*pwd;
	
	if (token == NULL || env_list == NULL)
		return (1);
	if (token->next != NULL)
	{
		printf("pwd: too many arguments\n");
		return (1);
	}
	pwd = get_env_value("PWD", env_list);
	if (pwd == NULL)
	{
		printf("pwd: PWD not set\n");
		return (1);
	}
	if (printf("%s\n", pwd) == -1)
		return (1);
	return (0);
}