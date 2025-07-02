/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix_the_problem.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 14:28:45 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/02 14:36:41 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// int is_path(t_ast *node, t_env *env_list)
// {
//     char *path;

//     path = check_for_var("PATH", env_list);
//     if (path)
//         execute_simple_command(node, env_list);
// }

// int is_dir(t_ast *node, t_env *env_list)
// {

// }

// int is_di_or_builtin(t_ast *node, t_env *env_list)
// {
//     if(if_builtin(node->left->token_list->value))
//         return (excute_simple_command(node, env_list));
//     else
//         return (is_dir(node, env_list));
// }