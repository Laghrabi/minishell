/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:20:21 by claghrab          #+#    #+#             */
/*   Updated: 2025/05/30 12:19:55 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cmd_or_builtin(t_token *token, t_env *env_list)
{
	int flag;
	
	if (token == NULL)
		return ;
	//expansion();
	flag = if_builtin(token->value);
	if (if_builtin(token->value) == 0)
		exec_cmd();
	else
		which_one(flag, token, env_list);
}

void	which_one(int flag, t_token *token, t_env *env_list)
{
	if (flag == 1)
		builtin_cd(token, env_list);
	else if (flag == 2)
		builtin_echo(token);
	else if (flag == 3)
		builtin_pwd(token, env_list);
	else if (flag == 4)
		builtin_export(token);
	else if (flag == 5)
		builtin_unset(token);
	else if (flag == 6)
		builtin_env(token);
	else if (flag == 7)
		builtin_exit(token);
}

int	if_builtin(char *cmd)
{
	if (cmd == NULL)
		return (0);
	if (ft_cmp(cmd, "cd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "echo") == 0)
		return (2);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (3);
	else if (ft_strcmp(cmd, "export") ==0)
		return (4);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (5);
	else if (ft_strcmp(cmd, "env") == 0)
		return (6);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (7);
	else
		return (0);
}

char    *get_env_value(char *key, t_env *env_list)
{
    t_env   *current;
    
    if (key == NULL || env_list == NULL)
        return (NULL);
    current = env_list;
    while (current != NULL)
    {
		if (ft_strcmp(key, current->key) == 0)
			return (current->value);
		current = current->next;
    }
	return (NULL);
}

void	update_env(char *key, char *new_value, t_env *env_list)
{
	t_env	*current;

	if (key == NULL || new_value == NULL || env_list == NULL)
		return ;
	current = env_list;
	while (current != NULL)
	{
		if (ft_strcmp(key, current->key) == 0)
		{
			if (current->value != NULL)
				free(current->value);
			current->value = ft_strdup(new_value);
			return ;
		}
		current = current->next;
	}
	return ;
}
