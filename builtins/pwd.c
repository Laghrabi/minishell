/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:48:27 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/11 09:34:39 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_pwd(t_token *token, t_env *env_list)
{
	char	*pwd;
	
	if (token == NULL)
		return (1);
	pwd = get_env_value("PWD", env_list);
	if (pwd != NULL)
	{
		printf("%s\n", pwd);
		update_env("_", "pwd", env_list);
		return (0);
	}
	pwd = getcwd(NULL, 0);
	if (pwd != NULL)
	{
		printf("%s\n", pwd);
		free(pwd);
		update_env("_", "pwd", env_list);
		return (0);
	}
	return (0);
}
