/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 10:27:21 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/12 01:27:45 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_last_command(t_ast *node, t_env *env_list, int pipe_read_end)
{
	pid_t	pid;
	int		exit_code;

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
		dup2(pipe_read_end, STDIN_FILENO);
		close(pipe_read_end);
		exit_code = execute_command(node, env_list);
		memory_management(env_list, 1);
		exit(exit_code);
	}
	return (pid);
}

int	prepare_pipe_execution(t_ast *node, t_env *env_list, t_pipe **pip)
{
	if (!node)
		return (1);
	*pip = initialize_pipe();
	if (!*pip)
		return (1);
	expand_evrything(node, env_list);
	if (!node || node->type != NODE_PIPE)
		return (execute_command(node, env_list));
	return (0);
}

int	handle_pipe_sequence(t_ast **node, t_pipe *pip, t_env *env_list,
		t_mul var)
{
	while (*node && (*node)->type == NODE_PIPE)
	{
		if (pipe(pip->cur_pipe) == -1)
		{
			perror("pipe");
			return (1);
		}
		var.pids[(*var.cmd_count)++] = fork_child_for_pipe((*node)->left,
				env_list, *var.input_fd, pip->cur_pipe);
		close(pip->cur_pipe[1]);
		if (*var.input_fd != STDIN_FILENO)
			close(*var.input_fd);
		*var.input_fd = pip->cur_pipe[0];
		pip->tmp = pip->prev_pipe;
		pip->prev_pipe = pip->cur_pipe;
		if (pip->tmp == pip->pipe_a)
			pip->cur_pipe = pip->pipe_b;
		else
			pip->cur_pipe = pip->pipe_a;
		*node = (*node)->right;
	}
	return (0);
}

int	handle_last_pipe_command(t_ast *node, t_pipe *pip, t_env *env_list,
		t_mul var)
{
	if (node && node->type == NODE_CMD)
	{
		var.pids[(*(var.cmd_count))++] = execute_last_command(node, env_list,
				*(var.input_fd));
		if (pip->prev_pipe && pip->prev_pipe[0] != STDIN_FILENO)
			close(pip->prev_pipe[0]);
	}
	return (*(var.cmd_count));
}

int	execute_pipe(t_ast *node, t_env *env_list, int input_fd)
{
	t_pipe	*pip;
	int		pids[1024];
	int		cmd_count;

	pip = NULL;
	cmd_count = 0;
	cmd_count = num_of_pipes(node);
	if (cmd_count >= 1000)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (2);
	}
	cmd_count = 0;
	if (!node || node->type != NODE_PIPE)
		return (prepare_pipe_execution(node, env_list, &pip));
	if (handle_pipe_sequence(&node, pip, env_list, (t_mul){pids, &cmd_count,
			&input_fd}) != 0)
		return (1);
	handle_last_pipe_command(node, pip, env_list,
		(t_mul){pids, &cmd_count, &input_fd});
	return (handle_wait_and_status(pids, cmd_count));
}
