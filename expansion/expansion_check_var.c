/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_check_var.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 20:41:11 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/08 20:50:52 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*handle_default_dollar(void)
{
	return (ft_strdup("$"));
}

char	*handle_digit_var(int *index)
{
	*index += 1;
	return (ft_strdup(""));
}

char	*handle_alpha_var(char *token, t_env *env, int *index)
{
	size_t	i;
	char	*var_name;
	char	*value;

	i = 0;
	while (ft_isalnum(token[i]) || token[i] == '_')
		i++;
	var_name = ft_substr(token, 0, i);
	value = get_env_value(var_name, env);
	if (value)
		*index += ft_strlen(var_name);
	return (ft_strdup(value));
}

char	*handle_exit_status(int *index)
{
	char	*exit_status;

	exit_status = ft_itoa(s_var()->exit_status);
	*index += 1;
	return (exit_status);
}

char	*check_var(char *token, t_env *env, int *index)
{
	if (token[0] == '?')
		return (handle_exit_status(index));
	if (ft_isalpha(token[0]) || token[0] == '_')
		return (handle_alpha_var(token, env, index));
	if (ft_isdigit(token[0]))
		return (handle_digit_var(index));
	return (handle_default_dollar());
}
