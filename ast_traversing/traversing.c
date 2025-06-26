/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traversing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:01:47 by claghrab          #+#    #+#             */
/*   Updated: 2025/06/26 16:53:41 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_ast(t_ast *node, t_env *env_list)
{
    if (!node)
	{	
       return (EXIT_FAILURE);
	}

	// printf("%s\n", node->token_list->value);
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

int execute_command(t_ast *node, t_env *env_list)
{
	// printf("%s\n", node->token_list->value);
    return cmd_or_builtin(node->left->token_list, env_list);
}
