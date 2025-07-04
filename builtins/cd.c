/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:47:58 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/04 23:53:55 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
    token = token->next;
    if (token->next != NULL)
    {
        ft_putstr_fd("bash: cd: too many arguments\n", 2);
        return (1);
    }
    else
    update_env("_", token->value, env_list);
	if (token == NULL || ft_strcmp(token->value, "~") == 0)
		new_path = get_env_value("HOME", env_list);
	else if (ft_strcmp(token->value, "-") == 0)
		new_path = get_env_value("OLDPWD", env_list);
	else
	{
		if (token->value[0] == '\0')
			new_path = ".";
		else
			new_path = token->value;
	}
	if (cd_helper(new_path, env_list) == 1)
		return (1);
	return (0);
}
