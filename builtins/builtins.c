/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 14:16:51 by claghrab          #+#    #+#             */
/*   Updated: 2025/06/27 03:09:38 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_echo(t_token *token)
{
	int	(i), (nl);
	nl = 0;
	if (token == NULL)
		return (1);
	token = token->next;
	if (token != NULL && ft_strcmp(token->value, "-n") == 0)
	{
		token = token->next;
		nl = 1;
	}
	while (token != NULL)
	{
		i = 0;
		while (token->value[i] != '\0')
		{
			if (token->value[i] != '\'' && token->value[i] != '\"')
				if (write(1, &token->value[i], 1) == -1)
					return (1);
			i++;
		}
		if (token->next != NULL && write(1, " ", 1) == -1)
			return (1);
		token = token->next;
	}
	if (nl == 0 && write(1, "\n", 1) == -1)
		return (1);
	return (0);
}

int	cd_helper(char *new_path, t_env *env_list)
{
	char	*old_pwd;
	char	*new_pwd;
	
	if (new_path == NULL || env_list == NULL)
		return (1);
	old_pwd = getcwd(NULL, 0);
	if (chdir(new_path) == -1)
	{
		printf("cd: %s: No such file or directory\n", new_path);
		free(old_pwd);
		garbage_collector(NULL, 1);
		return (1);
	}
	new_pwd = getcwd(NULL, 0);
	update_env("OLDPWD", old_pwd, env_list);
	update_env("PWD", new_pwd, env_list);
	free(old_pwd);
	free(new_pwd);
	return (0);
}

int	builtin_cd(t_token *token, t_env *env_list)
{
	char	*new_path;
	
	if (token == NULL || env_list == NULL)
		return (1);
	if (token->next == NULL || ft_strcmp(token->next->value, "~") == 0)
		new_path = get_env_value("HOME", env_list);
	else if (ft_strcmp(token->next->value, "-") == 0)
		new_path = get_env_value("OLDPWD", env_list);
	else
	{
		if (token->next->value[0] == '\0')
			new_path = ".";
		else
			new_path = token->next->value;
	}
	if (cd_helper(new_path, env_list) == 1)
		return (1);
	return (0);
}

int	builtin_pwd(t_token *token, t_env *env_list)
{
	char	*pwd;
	
	if (token == NULL || env_list == NULL)
		return (1);
	if (token->next != NULL)
	{
		printf("pwd: too many arguments\n");
		return (1);
	}
	pwd = get_env_value("PWD", env_list);
	if (pwd == NULL)
	{
		printf("pwd: PWD not set\n");
		return (1);
	}
	if (printf("%s\n", pwd) == -1)
		return (1);
	return (0);
}
int	builtin_env(t_token *token, t_env *env_list)
{
	t_env *current;

	if (token == NULL || env_list == NULL)
		return (1);
	if (token->next != NULL)
	{
		printf("env: too many arguments\n");
		return (1);
	}
	current = env_list;
	while (current != NULL)
	{
		if (current->value != NULL)
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	return (0);
}

int	builtin_unset(t_token *token, t_env **env_list)
{
	t_env *current;

	if (token == NULL || env_list == NULL  || *env_list == NULL)
		return (1);
	token = token->next;
	while (token != NULL && ft_strcmp((*env_list)->key, token->value) == 0)
	{
		*env_list = (*env_list)->next;
		token = token->next;	
	}
	while (token != NULL)
	{
		current	= *env_list;
		while (current != NULL)
		{
			if (current->next != NULL && ft_strcmp(token->value, current->next->key) == 0)
			{
				current->next = current->next->next;
				break ;
			}
			current = current->next;
		}
		token = token->next;
	}
	return (0);
}

int if_all_num(char *str)
{
	int	i;

	if (str == NULL)
		return (1);
	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

long long	ft_stoi(const char *str)
{
	int		i;
	int		sign;
	long long	result;

	i = 0;
	sign = 0;
	result = 0;
	while (str[i] == 32 || (str[i] <= 13 && str[i] >= 9))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = 1;
		i++;
	}
	while (str[i] <= '9' && str[i] >= '0')
	{
		result = result * 10 + (str[i] - '0');
		if (result > (long long)LONG_MAX + sign)
			return (LONG_LONG_MAX);
		i++;
	}
	if (sign)
		result *= -1;
	return (result);
}

int	builtin_exit(t_token *token)
{
	long long	nbr;

	if (token == NULL)
		return (1);
	if (token->next == NULL)
	{
		printf("exit\n");
		exit(0);
	}
	else if (token->next != NULL)
	{
		token = token->next;
		if (token->next != NULL)
		{
			printf("exit\nbash: exit: too many arguments\n");
			return (1);
		}
		if (if_all_num(token->value) == 1)
			return (1);
		nbr = ft_stoi(token->value);
		if (nbr == LONG_LONG_MAX)
		{
			printf("exit\nbash: exit: %s: numeric argument required\n", token->value);
			return (1);
		}
		else
		{
			printf("exit\n");
			exit(nbr % 256);
		}
	}
	return (1);
}

int	builtin_export(t_token *token, t_env **env_list)
{
	t_env	(*node);
	char	(*key), *value;
	int		(pos), (status);
	if (token == NULL || env_list == NULL  || *env_list == NULL)
		return (1);
	token = token->next;
	status = check_nm_var(token->value);
	if (status == 1 || status == 130)
		return (status);
	pos = find_chr_pos(token->value, '=');
	key = ft_substr(token->value, 0, pos);
	value = ft_substr(token->value, pos + 1, ft_strlen(token->value) - pos - 1);
	if (check_for_var(key, *env_list) == 0)
	{
		update_env(key, value, *env_list);
		return (0);
	}
	else
	{
		node = gc_malloc(sizeof(t_env));
		node->key = key;
		node->value = value;
		node->next = NULL;
		env_add_back(env_list, node);
		return (0);
	}
	return (1);
}
