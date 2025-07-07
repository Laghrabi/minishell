/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:48:22 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/06 20:40:52 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**sort_env_list(t_env *env_list)
{
	char	(**array);
	char	(*tmp);
	int		(i), (j);
	if (env_list == NULL)
		return (NULL);
	array = convert_env_to_array(env_list);
	i = 0;
	j = 0;
	while (array[i + 1] != NULL)
	{
		j = i + 1;
		while (array[j] != NULL)
		{
			if (ft_strcmp(array[i], array[j]) > 0)
			{
				tmp = array[i];
				array[i] = array[j];
				array[j] = tmp;
			}
			j++;
		}
		i++;
	}
	return (array);
}

void	only_export(t_env *env_list)
{
	char	(**array);
	char	(*str);
	int		(i), (pos);
	if (env_list == NULL)
		return ;
	array = sort_env_list(env_list);
	i = 0;
	while (array[i] != NULL)
	{
		pos = find_chr_pos(array[i], '=');
		if (pos != -1)
		{
			str = ft_substr2(array[i], 0, pos + 1);
			printf("declare -x %s\"", str);
			str = ft_strchr(array[i], '=');
			printf("%s\"\n", str + 1);
		}
		else
			printf("declare -x %s\n", array[i]);
		i++;
	}
}

int	builtin_export(t_token *token, t_env **env_list)
{
	t_env	(*node);
	char	(*key), *value;
	int		(pos), (status);
	if (token == NULL || env_list == NULL  || *env_list == NULL)
		return (1);
	if (token->next == NULL)
	{
		only_export(*env_list);
		update_env("_", "export", *env_list);
		return (0);
	}
	token = token->next;
	while (token)
	{
		status = check_nm_var(token->value);
		if (status == 1 || status == 130)
			return (status);
		pos = find_chr_pos(token->value, '=');
		if (pos == -1)
		{
			key = ft_strdup2(token->value);
			value = NULL;
		}
		else
		{
			key = ft_substr2(token->value, 0, pos);
			value = ft_substr2(token->value, pos + 1, ft_strlen(token->value) - pos - 1);
		}
		if (check_for_var(key, *env_list) == 0 && value != NULL)
		{
			update_env(key, value, *env_list);
		}
		else if (check_for_var(key, *env_list) == 1)
		{
			node = malloc(sizeof(t_env));
			node->key = key;
			node->value = value;
			node->next = NULL;
			env_add_back(env_list, node);
		}
		if (token->next == NULL)
			update_env("_", token->value, *env_list);
		token = token->next;
	}
	return (0);
}
