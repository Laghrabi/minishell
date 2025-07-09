/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 10:27:21 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/09 12:42:27 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_wait_and_status(int pid[2], int *status)
{
	signal(SIGINT, SIG_IGN);
	waitpid(pid[0], NULL, 0);
	waitpid(pid[1], status, 0);
	setup_signals();
	if (WIFSIGNALED(*status))
		s_var()->exit_status = 128 + WTERMSIG(*status);
	else if (WIFEXITED(*status))
		s_var()->exit_status = WEXITSTATUS(*status);
	return (s_var()->exit_status);
}

int	fork_and_execute_pipe_left(t_ast *node, t_env *env_list, int input_fd,
		int pipefd[2])
{
	pid_t (pid);
	int (exit_code);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (input_fd != STDIN_FILENO)
		{
			dup2(input_fd, STDIN_FILENO);
			close(input_fd);
		}
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		exit_code = execute_command(node->left, env_list);
		memory_management(env_list, 1);
		exit(exit_code);
	}
	return (pid);
}

int	handle_right_pipe_cmd(t_ast *node, t_env *env_list, int pipe_read_end)
{
	int		status;
	pid_t	pid;

	status = 0;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(pipe_read_end);
		return (1);
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		dup2(pipe_read_end, STDIN_FILENO);
		close(pipe_read_end);
		status = execute_command(node->right, env_list);
		memory_management(env_list, 1);
		exit(status);
	}
	close(pipe_read_end);
	return (pid);
}

int	setup_pipe_and_fork_left(db_par par, int input_fd, int pipefd[2],
		pid_t *left_pid)
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (1);
	}
	*left_pid = fork_and_execute_pipe_left(par.node, par.env_list, input_fd,
			pipefd);
	if (*left_pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}
	close(pipefd[1]);
	if (input_fd != STDIN_FILENO)
		close(input_fd);
	return (0);
}

int	execute_pipe(t_ast *node, t_env *env_list, int input_fd)
{
	int		pipefd[2];
	int		status;
	pid_t	pid[2];

	pid[0] = -1;
	pid[1] = -1;
	status = 0;
	expand_evrything(node, env_list);
	if (!node || node->type != NODE_PIPE)
		return (execute_command(node, env_list));
	if (setup_pipe_and_fork_left((db_par){node, env_list}, input_fd, pipefd,
		&pid[0]) != 0)
		return (1);
	if (node->right && node->right->type == NODE_PIPE)
	{
		status = execute_pipe(node->right, env_list, pipefd[0]);
		waitpid(pid[0], NULL, 0);
	}
	else if (node->right && node->right->type == NODE_CMD)
	{
		pid[1] = handle_right_pipe_cmd(node, env_list, pipefd[0]);
		status = handle_wait_and_status(pid, &status);
	}
	return (status);
}
