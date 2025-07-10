/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 14:31:31 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/10 01:29:31 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_variabls *s_var (void)
{
    static t_variabls global = {0};

    return (&global);
}

void	free_env(t_env *env)
{
	t_env	*current;

	while (env)
	{
		current = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = current;
	}
}

void	memory_management(t_env *env, int is_free_env)
{
	garbage_collector(NULL, 1);
	if (is_free_env)
	{
		free_env(env);
	}
}
