/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_error_messages.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 14:28:45 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/09 17:51:57 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_executable(t_ast *node, t_env *env_list)
{
	if (!access(node->left->token_list->value, X_OK))
		return (handle_simple_command(node, env_list, NULL));
	else
	{
		perror(node->left->token_list->value);
		if (node->right)
		{
			node->left = NULL;
			handle_simple_command(node, env_list, NULL);
		}
		return (126);
	}
	if (node->right)
		return (2);
}

int	is_dir(t_ast *node, t_env *env_list)
{
	struct stat	info;

	if (access(node->left->token_list->value, F_OK) == -1)
	{
		perror(node->left->token_list->value);
		node->left = NULL;
		if (node->right)
			handle_simple_command(node, env_list, NULL);
		return (127);
	}
	if (stat(node->left->token_list->value, &info) == 0
		&& S_ISDIR(info.st_mode))
	{
		ft_putstr_fd(node->left->token_list->value, 2);
		ft_putstr_fd(" : Is a directory\n", 2);
		node->left->token_list->is_already_exec = 1;
		if (node->right)
			handle_simple_command(node, env_list, NULL);
		return (126);
	}
	else
		return (is_executable(node, env_list));
	return (0);
}

int	is_absolute_path(t_ast *node, t_env *env_list)
{
	if (node->left && node->left->token_list
		&& ft_strchr(node->left->token_list->value, '/') != NULL)
		return (is_dir(node, env_list));
	else
		return (handle_simple_command(node, env_list, NULL));
	return (0);
}

int	is_di_or_builtin(t_ast *node, t_env *env_list, int i)
{
	if (!node || !env_list)
		return (1);
	if (node->left && node->left->token_list
		&& if_builtin(node->left->token_list->value))
		return (handle_simple_command(node, env_list, NULL));
	else if (i == 1)
		return (is_absolute_path(node, env_list));
	else
	{
		return (is_dir1(node, env_list));
	}
	return (1);
}

int	is_path(t_ast *node, t_env *env_list)
{
	int	path;

	if (node == NULL || env_list == NULL)
		return (1);
	path = check_for_var("PATH", env_list);
	if (!path)
		return (is_di_or_builtin(node, env_list, 1));
	else
		return (is_di_or_builtin(node, env_list, 0));
	return (0);
}
