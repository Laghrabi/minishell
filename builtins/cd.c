/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:47:58 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/05 23:56:17 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	replace_variable(int *flag, t_env *env_list, t_token *token)
{
	if (env_list == NULL)
		return ;
	if (*flag == 0)
	{
		if (token == NULL)
			update_env("_", "cd", env_list);
		else
			update_env("_", get_env_value("HOME", env_list), env_list);
	}
	else if (*flag == 1)
		update_env("_", "-", env_list);
	else
	{
		if (token->value[0] == '\0')
			update_env("_", "", env_list);
		else
			update_env("_", token->value, env_list);
	}
	return ;
}

static int handle_too_many_args(t_token *token, t_env *env_list)
{
    if (token && token->next != NULL)
    {
        while (token->next != NULL)
            token = token->next;
        update_env("_", token->value, env_list);
        ft_putstr_fd("bash: cd: too many arguments\n", 2);
        return 1;
    }
    return 0;
}

static void determine_path(t_token *token, t_env *env_list, 
                         char **new_path, int *flag)
{
    if (token == NULL || ft_strcmp(token->value, "~") == 0)
    {
        *flag = 0;
        *new_path = get_env_value("HOME", env_list);
    }
    else if (ft_strcmp(token->value, "-") == 0)
    {
        *flag = 1;
        *new_path = get_env_value("OLDPWD", env_list);
    }
    else
    {
        *flag = 2;
        *new_path = (token->value[0] == '\0') ? "." : token->value;
    }
}

int builtin_cd(t_token *token, t_env *env_list)
{
    char *new_path;
    int flag;

    if (token == NULL || env_list == NULL)
 		return (1);
    token = token->next;
    if (handle_too_many_args(token, env_list))
        return (1);
    determine_path(token, env_list, &new_path, &flag);
    replace_variable(&flag, env_list, token);
    if (cd_helper(new_path, env_list) == 1)
        return (1);
    return (0);
}

// int	builtin_cd(t_token *token, t_env *env_list)
// {
// 	char (*new_path), (*dash_var);
// 	int (flag);
// 	if (token == NULL || env_list == NULL)
// 		return (1);
//     token = token->next;
//     if (token->next != NULL)
//     {
// 		while (token->next->next != NULL)
// 			token = token->next;
// 		update_env("_", token->value, env_list);
//         ft_putstr_fd("bash: cd: too many arguments\n", 2);
//         return (1);
//     }
// 	if (token == NULL || ft_strcmp(token->value, "~") == 0)
// 	{
// 		flag = 0;
// 		new_path = get_env_value("HOME", env_list);
// 	}
// 	else if (ft_strcmp(token->value, "-") == 0)
// 	{
// 		flag = 1;
// 		new_path = get_env_value("OLDPWD", env_list);
// 	}
// 	else
// 	{
// 		flag = 2;
// 		if (token->value[0] == '\0')
// 			new_path = ".";
// 		else
// 			new_path = token->value;
// 	}
// 	replace_variable(&flag, env_list, token);
// 	if (cd_helper(new_path, env_list) == 1)
// 		return (1);
// 	return (0);
// }
