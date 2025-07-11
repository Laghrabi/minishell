/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 10:27:21 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/11 22:01:43 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_wait_and_status(int pid[1024], int cmd_count)
{
	int i;
	int status;
	
	signal(SIGINT, SIG_IGN);
	i = 0;
	while (i < cmd_count)
		waitpid(pid[i++], &status, 0);
	setup_signals();
	if (WIFSIGNALED(status))
		s_var()->exit_status = 128 + WTERMSIG(status);
	else if (WIFEXITED(status))
		s_var()->exit_status = WEXITSTATUS(status);
	return (s_var()->exit_status);
}

typedef struct s_pipes
{
	int pipe_a[2];
	int pipe_b[2];
	int *cur_pipe;
	int *prev_pipe;
	int	*tmp;
} t_pipe;

t_pipe *initialize_pipe(void)
{
	t_pipe *pipe = gc_malloc(sizeof(t_pipe));
	if (!pipe)
		return (NULL);
	pipe->cur_pipe = pipe->pipe_a;
	pipe->prev_pipe = NULL;
	pipe->tmp = NULL;
	return (pipe);
}

int	fork_child_for_pipe(t_ast *node, t_env *env_list, int input_fd,
		int pipefd[2])
{
	pid_t pid;
	int exit_code;
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
		close(pipefd[1]);
		close(pipefd[0]);
		exit_code = execute_command(node, env_list);
		memory_management(env_list, 1);
		exit(exit_code);
	}
	return (pid);
}

int	execute_last_command(t_ast *node, t_env *env_list, int pipe_read_end)
{
	pid_t pid;
	int exit_code;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		// close(pipe_read_end);
		return (-1);
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		dup2(pipe_read_end, STDIN_FILENO);
		close(pipe_read_end);
		exit_code = execute_command(node, env_list);
		memory_management(env_list, 1);
		exit(exit_code);
	}
	return (pid);
}

int	execute_pipe(t_ast *node, t_env *env_list, int input_fd)
{
	t_pipe *pip;
	int pids[1024];
	int cmd_count;
	
	pip = initialize_pipe();
	cmd_count = 0;
	expand_evrything(node, env_list);
	if (!node || node->type != NODE_PIPE)
		return (execute_command(node, env_list));
	while (node && node->type == NODE_PIPE)
	{
		if (pipe(pip->cur_pipe) == -1)
		{
			perror("pipe");
			return (1);
		}
		pids[cmd_count++] = fork_child_for_pipe(node->left, env_list, input_fd, pip->cur_pipe);
		close(pip->cur_pipe[1]);
		if (input_fd != STDIN_FILENO)
			close(input_fd);
		input_fd = pip->cur_pipe[0];
		pip->tmp = pip->prev_pipe;
		pip->prev_pipe = pip->cur_pipe;
		if (pip->tmp == pip->pipe_a)
			pip->cur_pipe = pip->pipe_b;
		else
			pip->cur_pipe = pip->pipe_a;
		node = node->right;
	}
	if (node && node->type == NODE_CMD)
	{
		pids[cmd_count++] = execute_last_command(node, env_list, input_fd);
		if (pip->prev_pipe && pip->prev_pipe[0] != STDIN_FILENO)
			close(pip->prev_pipe[0]);
	}
	return (handle_wait_and_status(pids, cmd_count));
}
