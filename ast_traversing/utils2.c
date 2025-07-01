/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 05:38:25 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/01 22:48:07 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char **convert_env_to_array(t_env *env_list)
{
    char (**argv);
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
    argv = gc_malloc(sizeof(char *) * (i + 1));
    i = 0;
    current = env_list;
    while (current != NULL)
    {
        argv[i] = ft_strdup(current->key);
        argv[i] = ft_strjoin(argv[i], "=");
        argv[i] = ft_strjoin(argv[i], current->value);
        i++;
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