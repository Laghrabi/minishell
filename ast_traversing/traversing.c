/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traversing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:01:47 by claghrab          #+#    #+#             */
/*   Updated: 2025/06/28 03:05:57 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_ast(t_ast *node, t_env *env_list)
{
    if (!node)	
       return (EXIT_FAILURE);

    if (node->type == NODE_CMD)
        return execute_command(node, env_list);
    // else if (node->type == NODE_PIPE)
    //     return execute_pipe(node, env_list);
    // else if (node->type == NODE_AND)
    //     return execute_and(node, env_list);
    // else if (node->type == NODE_OR)
    //     return execute_or(node, env_list);
    // else if (node->type == NODE_SUBSHELL)
    //     return execute_subshell(node, env_list);
    return (EXIT_FAILURE);
}

void	fd_leaks(int fd1, int fd2)
{
	dup2(fd1, STDOUT_FILENO);
	dup2(fd2, STDIN_FILENO);
	close(fd1);
	close(fd2);
}

int execute_command(t_ast *node, t_env *env_list)
{
    char (**argv);
    int	(saved_stdout), (saved_stdin), (status);
    argv = token_list_to_argv(node->left->token_list);
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
