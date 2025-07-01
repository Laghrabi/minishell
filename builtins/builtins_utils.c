/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:20:21 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/01 15:20:24 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int execute_simple_cmd(t_env *env_list, char **argv)
{
    pid_t pid;
    int status;
    char **envp;
    char *cmd_path;

	if (env_list == NULL || argv == NULL || *argv == NULL)
		return (1);
	//////3ndek chiiii MOCHKIIILE HNA BABE	
	if (ft_strchr(argv[0], '/') != NULL)
		cmd_path = argv[0];
	else
		cmd_path = find_cmd_path(argv[0], env_list);
	if (cmd_path == NULL)
	{
		printf("%s: command not found\n", argv[0]);
		s_var()->exit_status = 127;
		return (127);
	}
	envp = convert_env_to_array(env_list);
    pid = fork();
    if (pid == 0)
    {
        execve(cmd_path, argv, envp);
        perror("execve");
		//free
        exit(1);
    }
    else if (pid < 0)
    {
        perror("fork");
        free(cmd_path);
        return (1);
    }
    waitpid(pid, &status, 0);
	return (0);
}
int	cmd_or_builtin(t_token *token, t_env *env_list, char **argv)
{
	int flag;
	
	if (token == NULL || env_list == NULL || argv == NULL || *argv == NULL)
		return (-1);
	flag = if_builtin(token->value);
	if (flag == 0)
		return(execute_simple_cmd(env_list, argv));
	else
		return(which_one(flag, token, env_list));
	return (-1);
}

int	which_one(int flag, t_token *token, t_env *env_list)
{
	if (flag == 1)
		return(builtin_cd(token, env_list));
	else if (flag == 2)
		return(builtin_echo(token));
	else if (flag == 3)
		return(builtin_pwd(token, env_list));
	else if (flag == 4)
		return(builtin_export(token, &env_list));
	else if (flag == 5)
		return(builtin_unset(token, &env_list));
	else if (flag == 6)
		return(builtin_env(token, env_list));
	else if (flag == 7)
		return(builtin_exit(token));
	return (-1);
}

int	if_builtin(char *cmd)
{
	if (cmd == NULL)
		return (0);
	if (ft_strcmp(cmd, "cd") == 0)
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
			current->value = new_value;
			return ;
		}
		current = current->next;
	}
	return ;
}

int	check_nm_var(char *str)
{
	if (str == NULL)
		return (1);
	if (str[0] == '!')
	{
		printf("bash: %s: event not found\n", str);
		return (130);
	}
	else if ((str[0] < 'a' || str[0] > 'z') && (str[0] < 'A' || str[0] > 'Z') && str[0] != '_')
	{
		printf ("bash: export: `%s': not a valid identifier\n", str);
		return (1);
	}
	return (0);
}

int	check_for_var(char *key, t_env *env_list)
{
	t_env	*current;
	
	if (key == NULL || env_list == NULL)
		return (1);
	current = env_list;
	while (current != NULL)
	{
		if (ft_strcmp(key, current->key) == 0)
			return (0);
		current = current->next;
	}
	return (1);
}