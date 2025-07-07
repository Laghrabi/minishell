/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 05:38:25 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/07 22:05:59 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

char **convert_env_to_array(t_env *env_list)
{
    char (**argv), (*tmp);
    t_env (*current);
    int (i);
    if (env_list == NULL)
        return (NULL);
    i = 0;
    current = env_list;
    while (current != NULL)
    {
        i++;
        current = current->next;
    }
    argv = malloc(sizeof(char *) * i);
    i = 0;
    current = env_list;
    while (current != NULL)
    {
        if (ft_strcmp(current->key, "_") != 0)
        {
            argv[i] = ft_strdup2(current->key);
            if (current->value != NULL)
            {
                tmp = argv[i];
                argv[i] = ft_strjoin2(argv[i], "=");
                free(tmp);
                tmp = argv[i];
                argv[i] = ft_strjoin2(argv[i], current->value);  
                free(tmp);  
            }
            i++;
        }
        current = current->next;
    }
    argv[i] = NULL;
    return (argv);
}

char    **path_splitting(t_env *envp)
{
    char    **splited_path;
    char    *path;
    
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

// int	execute_cmd(t_env *env_list, char **argv)
// {
// 	char	*cmd_path;
// 	char	**envp;
// 	pid_t	pid;
	
// 	if (env_list == NULL || argv == NULL || *argv == NULL)
// 		return (1);
// 	cmd_path = find_cmd_path(argv[0], env_list);
// 	if (cmd_path == NULL)
// 	{
// 		printf("%s: command not found\n", argv[0]);
// 		return (127);
// 	}
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		envp = convert_env_to_array(env_list);
// 	}
	
// }