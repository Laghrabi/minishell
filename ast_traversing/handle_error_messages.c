/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_error_messages.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 14:28:45 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/02 21:49:30 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int is_executable(t_ast *node, t_env *env_list)
{
    if (!access(node->left->token_list->value, X_OK))
        return (handle_simple_command(node, env_list));
    else
    {
        printf("hi1\n");
        perror(node->left->token_list->value);
        return(126);
    }
    return (0);
}

int is_dir(t_ast *node, t_env *env_list)
{
    struct stat info;

    if (access(node->left->token_list->value, F_OK) == -1)
    {
        printf("hi2\n");
        perror(node->left->token_list->value);
        if (node->right)
            handle_simple_command(node, env_list);
        return (127);
    }
    else
    {
        if (stat(node->left->token_list->value, &info) == 0 && S_ISDIR(info.st_mode))
        {
            printf("%s : is a directory\n", node->left->token_list->value);
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

int not_found(t_ast * node, t_env *env_list)
{
    printf("%s; command not found\n", node->left->token_list->value);
    node->left->token_list->is_already_exec = 1;
    if (node->right)
        handle_simple_command(node, env_list);
    return (127);
}

int is_absolute_path(t_ast *node, t_env *env_list)
{
    // struct stat info;
    
    if (ft_strchr(node->left->token_list->value, '/') != NULL)
        return(is_dir(node, env_list));
    else
    {
        return (handle_simple_command(node, env_list));
        // if (!access(node->left->token_list->value, F_OK))
        // {
        //     if (stat(node->left->token_list->value, &info) == 0 && S_ISDIR(info.st_mode))
        //     {
        //         printf("%s; command not found\n", node->left->token_list->value);
        //         node->left->token_list->is_already_exec = 1;
        //         if (node->right)
        //             handle_simple_command(node, env_list);
        //         return (127);
        //     }
        //     else
        //         return (handle_simple_command(node, env_list));
        // }
        // else
        //     return (not_found(node , env_list));
    }
    return (0);
}

int is_di_or_builtin(t_ast *node, t_env *env_list)
{
    if (!node || !env_list)
        return (1);
    if(node->right || (node->left && if_builtin(node->left->token_list->value)))
        return (handle_simple_command(node, env_list));
    else
        return (is_absolute_path(node, env_list));
}
int is_path(t_ast *node, t_env *env_list)
{
    int path;

    path = check_for_var("PATH", env_list);
    if (!path)
        return (is_di_or_builtin(node, env_list));
    return (0);
}