/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:10:56 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/12 22:39:26 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_env2(char *key, char *new_value, t_env *env_list)
{
	t_env	*current;

	if (key == NULL || new_value == NULL || env_list == NULL)
		return ;
	current = env_list;
	while (current != NULL)
	{
		if (current->key && ft_strcmp(key, current->key) == 0)
		{
			if (current->value != NULL)
				free(current->value);
			current->value = ft_strdup2(new_value);
			return ;
		}
		current = current->next;
	}
	return ;
}

void	hardcode_envlist(char *key, char *value, t_env **head)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	node->key = ft_strdup2(key);
	node->value = ft_strdup2(value);
	node->next = NULL;
	node->prev = NULL;
	env_add_back(head, node);
}

t_env	*set_key_value(void)
{
	char	*pwd;
	char	*str;
	t_env	*head;

	head = NULL;
	pwd = getcwd(NULL, 0);
	hardcode_envlist("OLDPWD", NULL, &head);
	hardcode_envlist("PWD", pwd, &head);
	hardcode_envlist("SHLVL", "1", &head);
	str = ft_strjoin2(pwd, "./minishell");
	hardcode_envlist("_", str, &head);
	free(pwd);
	free(str);
	return (head);
}

void	increment_shelvl_value(t_env *envp)
{
	char	*string;
	int		integer;

	if (envp == NULL)
		return ;
	string = get_env_value("SHLVL", envp);
	integer = ft_atoi(string);
	integer++;
	string = ft_itoa(integer);
	update_env2("SHLVL", string, envp);
}

t_env	*init_env(char **envp)
{
	int		i;
	int		pos;
	t_env	*head;
	t_env	*node;

	if (envp == NULL || *envp == NULL)
		return (set_key_value());
	i = 0;
	head = NULL;
	while (envp[i] != NULL)
	{
		node = malloc(sizeof(t_env));
		pos = find_chr_pos(envp[i], '=');
		node->key = ft_substr2(envp[i], 0, pos);
		node->value = ft_substr2(envp[i], pos + 1, ft_strlen(envp[i]) - pos
				- 1);
		node->next = NULL;
		node->prev = NULL;
		env_add_back(&head, node);
		i++;
	}
	increment_shelvl_value(head);
	return (head);
}
