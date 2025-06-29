/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 10:27:21 by zfarouk           #+#    #+#             */
/*   Updated: 2025/06/29 19:56:37 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// int execute_compound_command(t_ast *node, t_env *env_list)
// {
    // int exit_status;
    
    // if (!node || !env_list)
    //     return (0);
    // if (node->type == NODE_AND || node->type == NODE_OR)
    //     exit_status = execute_pipe(node->left, env_list, 0);
    // if (node->type == NODE_AND)
    // {
    //     if (exit_status == 0)
    //         exit_status = execute_compound_command(node->left, env_list);
    //     else
    //         return (exit_status);
    // }
    // else if (node->type == NODE_OR)
    // {
    //     if (exit_status != 0)
    //         exit_status = execute_compound_command(node->left, env_list);
    //     else
    //         return (exit_status);
    // }
    // else
    //     execute_pipe(node, env_list, 0);
    // return (exit_status);



int execute_subshell(t_ast *node, t_env *env_list)
{
    pid_t pid;
    int status;

//     // if (!node)
//     // {
//     //     //testi hadi ➜  minishell git:(main) ✗ () && echo hello 
//     // }
    pid = fork();
    if (pid == -1)
    if (pid == 0)
    {
        if (node->right)
            setup_redirections(node->right);
        exit(execute_compound_command(node->left, env_list));
    }
    waitpid(pid, &status, 0);
    printf("%d--\n", WEXITSTATUS(status));
    return WEXITSTATUS(status);
}

// int execute_subshell(t_ast *node, t_env *env_list)
// {
//     if (node->left)
//         execute_compound_command(node->left, env_list);
//     if (node->right)
//         setup_redirections(node->right);
// }

void	fd_leaks(int fd1, int fd2)
{
	dup2(fd1, STDOUT_FILENO);
	dup2(fd2, STDIN_FILENO);
	close(fd1);
	close(fd2);
}

int execute_simple_command(t_ast *node, t_env *env_list)
{
    char (**argv);
    int	(saved_stdout), (saved_stdin), (status);
    //ana zdtha
    if (!node || !node->left || !node->left->token_list)
        return (1);
    argv = token_list_to_argv(node->left->token_list);
    if (!argv || !argv[0])
        return (1);
    if (node->right != NULL && (node->right->type == NODE_APPEND || node->right->type == NODE_OREDIR || node->right->type == NODE_IREDIR))
    {
        saved_stdout = dup(STDOUT_FILENO);
		saved_stdin = dup(STDIN_FILENO);
    	status = setup_redirections(node->right);
		if (status == 1)
		{
			fd_leaks(saved_stdin, saved_stdout);
			return (1);
		}
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
    {
        // printf("motherfuccker\n");
        return (execute_subshell(node, env_list));
    }
    else
        return (execute_simple_command(node , env_list));
}
int execute_pipe(t_ast *node, t_env *env_list, int input_fd)
{
    int pipefd[2];
    pid_t pid;
    if (!node)
        return (1);
    if (node->type != NODE_PIPE)
        return(execute_command(node, env_list));
    pipe(pipefd);
    pid = fork();
    // if (pid == -1)
        // strerror("failed to fork");
    if (pid == 0)
    {
        //khasni nchof ila kant awal fork
        // ma3ndhach lach dup liya si non 
        //khsni n9ra mn pipe l9dima
        if (input_fd != STDIN_FILENO)
        {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }
        dup2(pipefd[1], STDOUT_FILENO);
        //nsdo pipe fchild
        close(pipefd[0]);
        close(pipefd[1]);
        // printf("hello simple command");
        execute_command(node->left, env_list);
        exit(1);
    }
    wait(NULL);
    //maendna mandiro db b write flpipe[1] so ansdoha
    close(pipefd[1]);
    //3lach , 7itach input-fd awal mra ghaykon 0 whta stdin so ila closito ghanclosi hta stdin
    if (input_fd != STDIN_FILENO)
        close(input_fd);
    if (node->right && node->right->type == NODE_PIPE)
    {        
        int status = execute_pipe(node->right, env_list, pipefd[0]);
        if (status != 0)
            return status;
    }
    else if (node->right && node->right->type == NODE_CMD)
    {
        pid = fork();
        if (pid == 0)
        {
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            execute_command(node->right, env_list);
            exit(1);
        }
        close(pipefd[0]);
        wait(NULL);
    }
    else
        close(pipefd[0]);
    wait(NULL);
    return(0);
}

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
        return execute_pipe(node, env_list, STDIN_FILENO);
    return (1);
}