/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 05:38:25 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/12 22:38:54 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**token_list_to_argv(t_token *token_list)
{
	char	**argv;
	t_token	*current;
	int		i;

	if (token_list == NULL)
		return (NULL);
	i = 0;
	current = token_list;
	while (current != NULL)
	{
		i++;
		current = current->next;
	}
	argv = gc_malloc(sizeof(char *) * (i + 1));
	i = 0;
	current = token_list;
	while (current != NULL)
	{
		if (current->value[0] != '\0' || !current->ambiguous)
			argv[i++] = ft_strdup(current->value);
		current = current->next;
	}
	argv[i] = NULL;
	return (argv);
}

int	is_dir1(t_ast *node, t_env *env_list)
{
	if (node->left && node->left->token_list)
		return (is_dir(node, env_list));
	else if (node->right)
		return (handle_simple_command(node, env_list, NULL));
	return (0);
}

char	*ft_strjoin2(char const *s1, char const *s2)
{
	size_t	j;
	size_t	i;
	char	*t;

	if (!s1 || !s2)
		return (NULL);
	t = (char *)malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (!t)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i])
		t[j++] = s1[i++];
	i = 0;
	while (s2[i])
		t[j++] = s2[i++];
	t[j] = 0;
	return (t);
}

char	**path_splitting(t_env *envp)
{
	char	**splited_path;
	char	*path;

	if (envp == NULL)
		return (NULL);
	path = get_env_value("PATH", envp);
	if (path == NULL)
		return (NULL);
	splited_path = ft_split(path, ':');
	if (splited_path == NULL || *splited_path == NULL)
		return (NULL);
	return (splited_path);
}

char	*find_cmd_path(char *cmd, t_env *env_list)
{
	int		i;
	char	*full_path;
	char	*with_slash;
	char	**paths;

	if (env_list == NULL || cmd == NULL)
		return (NULL);
	i = 0;
	paths = path_splitting(env_list);
	while (paths && paths[i] != NULL)
	{
		with_slash = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(with_slash, cmd);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		i++;
	}
	return (NULL);
}
