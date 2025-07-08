/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_compound_command.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 22:48:18 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/08 23:20:57 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int handle_node_and(t_ast *node, t_env *env_list)
{
    int exit_status;

    exit_status = execute_pipe(node->left, env_list, STDIN_FILENO);
    if (exit_status >= 130)
        write(1, "\n", 1);
    if (exit_status == 0)
        return (execute_compound_command(node->right, env_list));
    while (node && node->right)
    {
        if (node->right->type == NODE_OR)
        {
            if (node->right->right)
                execute_compound_command(node->right->right, env_list);
            break;
        }
        node = node->right;
    }
    return (exit_status);
}

int handle_node_or(t_ast *node, t_env *env_list)
{
    int exit_status;

    exit_status = execute_pipe(node->left, env_list, STDIN_FILENO);
    if (exit_status >= 130)
        write(1, "\n", 1);
    if (exit_status != 0)
        return (execute_compound_command(node->right, env_list));
    while (node && node->right)
    {
        if (node->right->type == NODE_AND)
        {
            if (node->right->right)
                execute_compound_command(node->right->right, env_list);
            break;
        }
        node = node->right;
    }
    return (exit_status);
}

int execute_compound_command(t_ast *node, t_env *env_list)
{
    int exit_status;

    if (!node || !env_list)
        return (0);
    if (node->type == NODE_AND)
        return (handle_node_and(node, env_list));
    else if (node->type == NODE_OR)
        return (handle_node_or(node, env_list));
    exit_status = execute_pipe(node, env_list, STDIN_FILENO);
    if (exit_status >= 130)
        write(1, "\n", 1);
    return (exit_status);
}

// int execute_compound_command(t_ast *node, t_env *env_list)
// {
//     int exit_status;

//     if (!node || !env_list)
//         return (0);
//     if (node->type == NODE_AND)
//     {
//         exit_status = execute_pipe(node->left, env_list, STDIN_FILENO);
//         if (exit_status >= 130)
//             write(1, "\n", 1);
//         if (exit_status == 0)
//             exit_status = execute_compound_command(node->right, env_list);
//         else
//         {
//             while (node && node->right)
//             {
//                 if (node->right->type == NODE_OR)
//                 {
//                     if (node->right->right)
//                         execute_compound_command(node->right->right, env_list);
//                     break;
//                 }
//                 node = node->right;
//             }
//         }
//         return (exit_status);
//     }
//     else if (node->type == NODE_OR)
//     {
//         exit_status = execute_pipe(node->left, env_list, STDIN_FILENO);
//         if (exit_status >= 130)
//             write(1, "\n", 1);
//         if (exit_status != 0)
//             exit_status = execute_compound_command(node->right, env_list);
//         else
//         {
//             while (node && node->right)
//             {
//                 if (node->right->type == NODE_AND)
//                 {
//                     if (node->right->right)
//                         execute_compound_command(node->right->right, env_list);
//                     break;
//                 }
//                 node = node->right;
//             }
//         }
//         return (exit_status);
//     }
//     else
//     {
//         exit_status = execute_pipe(node, env_list, STDIN_FILENO);
//         if (exit_status >= 130)
//             write(1, "\n", 1);
//         return (exit_status);
//     }
//     return (1);
// }