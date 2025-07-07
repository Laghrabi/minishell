/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:48:10 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/07 22:21:07 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
			current->value = ft_strdup2(new_value);
			return ;
		}
		current = current->next;
	}
	return ;
}

int	check_nm_var(char *str)
{
	int	i;
	
	if (str == NULL)
		return (1);
	i = 0;
	while (str[i])
	{
		if (i != 0 && str[i] == '=')
			break ;
		if ((str[i] < 'a' || str[i] > 'z') && (str[i] < 'A' || str[i] > 'Z') && str[i] != '_')
		{
			printf ("bash: export: `%s': not a valid identifier\n", str);
			return (1);
		}
		i++;
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
	update_env("_", "env", env_list);
	current = env_list;
	while (current != NULL)
	{
		if (current->value != NULL)
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	return (0);
}
