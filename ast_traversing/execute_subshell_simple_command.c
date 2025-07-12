/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_subshell_simple_command.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 22:58:36 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/12 01:19:28 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	num_of_pipes(t_ast *node)
{
	int	count;

	count = 0;
	while (node && node->type == NODE_PIPE)
	{
		count++;
		node = node->right;
	}
	return (count);
}

void	fd_leaks(int fd1, int fd2)
{
	dup2(fd1, STDIN_FILENO);
	dup2(fd2, STDOUT_FILENO);
	close(fd1);
	close(fd2);
}

int	execute_subshell(t_ast *node, t_env *env_list)
{
	int	status;
	int	saved_stdout;
	int	saved_stdin;
	int	redirected;

	redirected = 0;
	if (!node->left)
		return (2);
	if (node->right)
	{
		saved_stdout = dup(STDOUT_FILENO);
		saved_stdin = dup(STDIN_FILENO);
		redirected = 1;
		if (setup_redirections(node->right, env_list) == 1)
		{
			close(saved_stdout);
			close(saved_stdin);
			return (1);
		}
	}
	status = execute_compound_command(node->left, env_list);
	if (redirected)
		fd_leaks(saved_stdin, saved_stdout);
	return (status);
}

int	handle_simple_command(t_ast *node, t_env *env_list, char **argv)
{
	int (saved_stdout), (saved_stdin), (status);
	if (!node)
		return (1);
	if (node->left && node->left->token_list)
		argv = token_list_to_argv(node->left->token_list);
	if (node->right != NULL && (node->right->type == NODE_APPEND
			|| node->right->type == NODE_OREDIR
			|| node->right->type == NODE_IREDIR
			|| node->right->type == NODE_HEREDOC))
	{
		saved_stdout = dup(STDOUT_FILENO);
		saved_stdin = dup(STDIN_FILENO);
		status = setup_redirections(node->right, env_list);
		if (status == 1)
		{
			fd_leaks(saved_stdin, saved_stdout);
			return (1);
		}
		if (node->left != NULL && node->left->token_list
			&& node->left->token_list->is_already_exec == 0)
			status = cmd_or_builtin(node->left->token_list, env_list, argv);
		fd_leaks(saved_stdin, saved_stdout);
		return (status);
	}
	return (cmd_or_builtin(node->left->token_list, env_list, argv));
}

int	execute_command(t_ast *node, t_env *env_list)
{
	if (!node)
		return (1);
	if (node->type == NODE_SUBSHELL)
		return (execute_subshell(node, env_list));
	else
		return (is_path(node, env_list));
	return (1);
}
