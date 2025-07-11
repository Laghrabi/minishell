/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 17:40:29 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/11 22:33:45 by zfarouk          ###   ########.fr       */
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

char	*get_command_path(char *cmd, t_env *env_list)
{
	char	*cmd_path;

	if (ft_strchr(cmd, '/') != NULL)
		return (cmd);
	cmd_path = find_cmd_path(cmd, env_list);
	if (!cmd_path && check_for_var("PATH", env_list))
		cmd_path = ft_strjoin("./", cmd);
	return (cmd_path);
}

int	handle_exec_failure(t_env *env_list, char *cmd_path, char **argv)
{
	if (cmd_path == NULL && !check_for_var("PATH", env_list))
	{
		ft_putstr_fd(argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		s_var()->exit_status = 127;
		return (127);
	}
	return (-1);
}

void	child_process_execution(char *cmd_path, char **argv, char **envp,
			t_env *env_list)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	execve(cmd_path, argv, envp);
	perror("execve");
	free_double_array(envp);
	memory_management(env_list, 1);
	if (errno == EACCES)
		exit(126);
	else if (errno == ENOENT)
		exit(127);
	else
		exit(1);
}

int	handle_parent_process(pid_t pid, int status)
{
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		status = 128 + WTERMSIG(status);
		if (status == 131)
			write(2, "Quit (core dumped)", 19);
	}
	else
		status = WEXITSTATUS(status);
	setup_signals();
	return (status);
}

int	execute_simple_cmd(t_env *env_list, char **argv)
{
	char	**envp;
	char	*cmd_path;

	int (status), (pid);
	if (env_list == NULL || argv == NULL || *argv == NULL)
		return (0);
	cmd_path = get_command_path(argv[0], env_list);
	status = handle_exec_failure(env_list, cmd_path, argv);
	if (status != -1)
		return (status);
	envp = convert_env_to_array(env_list);
	pid = fork();
	if (pid == 0)
		child_process_execution(cmd_path, argv, envp, env_list);
	else if (pid < 0)
	{
		perror("fork");
		free(cmd_path);
		return (1);
	}
	replace_last_executed_cmd(env_list, argv);
	signal(SIGINT, SIG_IGN);
	status = handle_parent_process(pid, status);
	free_double_array(envp);
	return (status);
}

// int execute_simple_cmd(t_env *env_list, char **argv)
// {
//     pid_t pid;
//     int status;
//     char **envp;
//     char *cmd_path;

// 	if (env_list == NULL || argv == NULL || *argv == NULL)
// 		return (0);
// 	if (ft_strchr(argv[0], '/') != NULL)
// 		cmd_path = argv[0];
// 	else
// 	{	
// 		cmd_path = find_cmd_path(argv[0], env_list);
// 		if (!cmd_path && check_for_var("PATH", env_list))
// 			cmd_path = ft_strjoin("./", argv[0]);
// 	}
// 	if (cmd_path == NULL && !check_for_var("PATH", env_list))
// 	{
// 		ft_putstr_fd(argv[0], 2);
// 		ft_putstr_fd(": command not found\n", 2);
// 		s_var()->exit_status = 127;
// 		return (127);
// 	}
// 	envp = convert_env_to_array(env_list);
//     pid = fork();
//     if (pid == 0)
//     {
// 		signal(SIGINT, SIG_DFL);
//         signal(SIGQUIT, SIG_DFL);
//         execve(cmd_path, argv, envp);
//         perror("execve");
//         free_double_array(envp);
// 		memory_management(env_list, 1);
// 		if (errno == EACCES)
// 			exit(126);
// 		else if (errno == ENOENT)
// 			exit(127);
// 		else
// 			exit(1);
//     }
//     else if (pid < 0)
//     {
//         perror("fork");
//         free(cmd_path);
//         return (1);
//     }
// 	replace_last_executed_cmd(env_list, argv);
//     signal(SIGINT, SIG_IGN);
//     waitpid(pid, &status, 0);
// 	if (WIFSIGNALED(status))
// 	{
//         status = 128 + WTERMSIG(status);
// 		if (status == 131)
// 			write(2, "Quit (core dumped)", 19);
// 	}
//     else
// 		status = WEXITSTATUS(status);
// 	free_double_array(envp);
//     setup_signals();
// 	return (status);
// }