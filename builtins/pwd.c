/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:48:27 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/06 18:57:06 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	update_env("_", "pwd", env_list);
	return (0);
}
