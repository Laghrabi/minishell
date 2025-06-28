/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 10:27:21 by zfarouk           #+#    #+#             */
/*   Updated: 2025/06/28 21:43:31 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int execute_pipe(t_ast *node, t_env *env_list, int input_fd)
{
    int pipefd[2];
    pid_t pid;
    if (!node)
        return (1);
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
        execute_command(node->left, env_list);
        exit(1);
    }
    wait(NULL);
    //maendna mandiro db b write flpipe[1] so ansdoha
    close(pipefd[1]);
    //3lach , 7itach input-fd awal mra ghaykon 0 whta stdin so ila closito ghanclosi hta stdin
    if (input_fd != STDIN_FILENO)
        close(input_fd);
    if (node->right->type == NODE_PIPE)
    {        
        int status = execute_pipe(node->right, env_list, pipefd[0]);
        if (status != 0)
            return status;
    }
    else if (node->right->type == NODE_CMD)
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
        wait(NULL); // right-most command
    }
    else
        close(pipefd[0]);
    wait(NULL);
    return(0);
}
