/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 10:27:21 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/06 02:57:18 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	fd_leaks(int fd1, int fd2)
{
	dup2(fd1, STDIN_FILENO);
	dup2(fd2, STDOUT_FILENO);
	close(fd1);
	close(fd2);
}

int execute_subshell(t_ast *node, t_env *env_list)
{
    pid_t pid;
    int status;
    int saved_stdout;
    int saved_stdin;

    if (!node->left)
       return (2);
    if (node->right) {
        saved_stdout = dup(STDOUT_FILENO);
        saved_stdin = dup(STDIN_FILENO);
        if (setup_redirections(node->right, env_list) == 1)
        {
            close(saved_stdout);
            close(saved_stdin);
            return 1;
        }
    }
    pid = fork();
    if (pid == -1) {
        perror("fork");
        fd_leaks(saved_stdin, saved_stdout);
        return 1;
    }
    else if (pid == 0) {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        status = execute_compound_command(node->left, env_list);
        memory_management(env_list, 1);
        exit(status);
    }
    signal(SIGINT, SIG_IGN);
    waitpid(pid, &status, 0);
    setup_signals();
    if (WIFSIGNALED(status))
        s_var()->exit_status = 128 + WTERMSIG(status);
    else if (WIFEXITED(status))
        s_var()->exit_status = WEXITSTATUS(status);
    fd_leaks(saved_stdin, saved_stdout);
    return (0);
}

int handle_simple_command(t_ast *node, t_env *env_list)
{
    char (**argv);
    int	(saved_stdout), (saved_stdin), (status);
    argv = NULL;
    if (!node)
        return (1);
    if (node->left && node->left->token_list)
        argv = token_list_to_argv(node->left->token_list);
    //ymklek tbli had lcondition kamla bdik li katchekilek wach raha redi
    if (node->right != NULL && (node->right->type == NODE_APPEND || node->right->type == NODE_OREDIR || node->right->type == NODE_IREDIR || node->right->type == NODE_HEREDOC))
    {
        saved_stdout = dup(STDOUT_FILENO);
		saved_stdin = dup(STDIN_FILENO);
        status = setup_redirections(node->right, env_list);
		if (status == 1)
		{
			fd_leaks(saved_stdin, saved_stdout);
			return (1);
		}
        if (node->left != NULL && node->left->token_list && node->left->token_list->is_already_exec == 0)
		        status = cmd_or_builtin(node->left->token_list, env_list, argv);
		fd_leaks(saved_stdin, saved_stdout);
		return (status);
    }
    return (cmd_or_builtin(node->left->token_list, env_list, argv));
}

int execute_command(t_ast *node, t_env *env_list)
{
    if (!node)
        return (1);
    if (node->type == NODE_SUBSHELL)
        return (1);//execute_subshell(node, env_list));
    else
        return (is_path(node , env_list));
}

void handle_wait_and_status(int pid, int *status)
{
    signal(SIGINT, SIG_IGN);
    waitpid(pid, status, 0);
    setup_signals();
    if (WIFSIGNALED(*status))
        s_var()->exit_status = 128 + WTERMSIG(*status);
    else if (WIFEXITED(*status))
        s_var()->exit_status = WEXITSTATUS(*status);
}

int fork_and_execute_pipe_left(t_ast *node, t_env *env_list, int input_fd, int pipefd[2]) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return (-1);
    }
    if (pid == 0) {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        if (input_fd != STDIN_FILENO) {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        int exit_code = execute_command(node->left, env_list);
        memory_management(env_list, 1);
        exit(exit_code);
    }
    
    return (pid);
}

int handle_right_pipe_cmd(t_ast *node, t_env *env_list, int pipe_read_end)
{
    int status;

    status = 0;
    pid_t pid = fork();
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
        int exit_code = execute_command(node->right, env_list);
        memory_management(env_list, 1);
        exit(exit_code);
    }
    close(pipe_read_end);
    handle_wait_and_status(pid, &
    
    status);
    return (0);
}



int execute_pipe(t_ast *node, t_env *env_list, int input_fd)
{
    int pipefd[2];
    pid_t pid;
    int status;

    expand_evrything(node, env_list);
    if (!node || node->type != NODE_PIPE)
        return execute_command(node, env_list);
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return (1);
    }
    pid = fork_and_execute_pipe_left(node, env_list, input_fd, pipefd);
    if (pid == -1)
        return (1);
    close(pipefd[1]);
    if (input_fd != STDIN_FILENO)
        close(input_fd);
    handle_wait_and_status(pid, &status);
    if (node->right && node->right->type == NODE_PIPE)
        return execute_pipe(node->right, env_list, pipefd[0]);
    else if (node->right && node->right->type == NODE_CMD)
        return handle_right_pipe_cmd(node, env_list, pipefd[0]);
    close(pipefd[0]);
    return (0);
}

// int execute_pipe(t_ast *node, t_env *env_list, int input_fd)
// {
//     int pipefd[2];
//     pid_t pid;
//     int status;
//     int exit_code;
    
//     if (!node)
//         return (1);
//     expand_evrything(node, env_list);
//     if (node->type != NODE_PIPE)
//         return(execute_command(node, env_list));
//     if (pipe(pipefd) == -1)
//     {
//         perror("pipe");
//         return (1);
//     }
//     pid = fork();
//     if (pid == -1)
//     {
//         perror("fork");
//         // close(pipefd[0]);
//         // close(pipefd[1]);
//         return(1);   
//     }
//     if (pid == 0)
//     {
//     	signal(SIGINT, SIG_DFL);
// 	    signal(SIGQUIT, SIG_DFL);
//         if (input_fd != STDIN_FILENO)
//         {
//             dup2(input_fd, STDIN_FILENO);
//             close(input_fd);
//         }
//         dup2(pipefd[1], STDOUT_FILENO);
//         close(pipefd[0]);
//         close(pipefd[1]);
//         exit_code = execute_command(node->left, env_list);
//         memory_management(env_list, 1);
//         exit(exit_code);
//     }
//     close(pipefd[1]);
//     if (input_fd != STDIN_FILENO)
//         close(input_fd);
//     signal(SIGINT, SIG_IGN);
//     waitpid(pid, &status, 0);
//     setup_signals();
//     if (WIFSIGNALED(status))
//         s_var()->exit_status = 128 + WTERMSIG(status);
//     else if (WIFEXITED(status))
//         s_var()->exit_status = WEXITSTATUS(status);
//     if (node->right && node->right->type == NODE_PIPE)
//         return (execute_pipe(node->right, env_list, pipefd[0]));
//     else if (node->right && node->right->type == NODE_CMD)
//     {
//         pid = fork();
//         if (pid == -1)
//         {
//             perror("fork");
//             close(pipefd[0]);
//             return (1);
//         }
//         if (pid == 0)
//         {
//             dup2(pipefd[0], STDIN_FILENO);
//             close(pipefd[0]);
//             int exit_code = execute_command(node->right, env_list);
//             memory_management(env_list, 1);
//             exit(exit_code);
//         }
//         close(pipefd[0]);
//         waitpid(pid, &status, 0);
//         if (WIFSIGNALED(status))
//             s_var()->exit_status = 128 + WTERMSIG(status);
//         else if (WIFEXITED(status))
//             s_var()->exit_status = WEXITSTATUS(status);
//     }
//     else
//         close(pipefd[0]);
//     return(0);
// }

int execute_compound_command(t_ast *node, t_env *env_list)
{
    int exit_status;

    if (!node || !env_list)
        return (0);
    if (node->type == NODE_AND)
    {
        exit_status = execute_pipe(node->left, env_list, STDIN_FILENO);
        if (exit_status == 0)
            exit_status = execute_compound_command(node->right, env_list);
        return (exit_status);
    }
    else if (node->type == NODE_OR)
    {
        exit_status = execute_pipe(node->left, env_list, STDIN_FILENO);
        if (exit_status != 0)
            exit_status = execute_compound_command(node->right, env_list);
        return (exit_status);
    }
    else
        return (execute_pipe(node, env_list, STDIN_FILENO));
    return (1);
}
