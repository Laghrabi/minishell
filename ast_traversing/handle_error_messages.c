/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix_the_problem.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 14:28:45 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/02 16:47:35 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int is_executable(t_ast *node, t_env *env_list)
{
    if (access(node->left->token_list->value, X_OK))
        return (handle_simple_command(node, env_list));
    else
    {
        perror(node->left->token_list->value);
        return(126);
    }
    return (0);
}

int is_dir(t_ast *node, t_env *env_list)
{
    struct stat info;

    if (!access(node->left->token_list->value, F_OK))
    {
        perror(node->left->token_list->value);
        if (node->right)
            handle_simple_command(node, env_list);
        return (127);
    }
    else
    {
        stat(node->left->token_list->value, &info);
        if (S_ISDIR(info.st_mode))
        {
            printf("%s : is a directory", node->left->token_list->value);
            node->left->token_list->is_already_exec = 1;
            if (node->right)
                handle_simple_command(node, env_list);
            return (126);
        }
        else
            return (is_executable(node, env_list));
    }
    return (0);
}

int is_absolute_path(t_ast *node, t_env *env_lsit)
{
    if (ft_strchr(node->left->token_list->value, '/'))
        return(is_dir(node, env_lsit));
    return (0);
}

int is_di_or_builtin(t_ast *node, t_env *env_list)
{
    if(if_builtin(node->left->token_list->value))
        return (handle_simple_command(node, env_list));
    else
        return (is_absolute_path(node, env_list));
}
int is_path(t_ast *node, t_env *env_list)
{
    int path;

    path = check_for_var("PATH", env_list);
    if (path)
        is_di_or_builtin(node, env_list);
    return (0);
}