/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 23:27:45 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/12 22:38:33 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	replace_last_executed_cmd(t_env *env_list, char **argv)
{
	int	i;

	if (env_list == NULL || argv == NULL || *argv == NULL)
		return ;
	i = 0;
	while (argv[i + 1] != NULL)
		i++;
	update_env("_", argv[i], env_list);
}

void	free_double_array(char **db_str)
{
	int	i;

	i = 0;
	while (db_str[i])
	{
		free(db_str[i]);
		i++;
	}
	free(db_str);
}

int	handle_wait_and_status(int pid[1024], int cmd_count)
{
	int	i;
	int	status;

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

t_pipe	*initialize_pipe(void)
{
	t_pipe	*pipe;

	pipe = gc_malloc(sizeof(t_pipe));
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
		close(pipefd[1]);
		close(pipefd[0]);
		exit_code = execute_command(node, env_list);
		memory_management(env_list, 1);
		exit(exit_code);
	}
	return (pid);
}
