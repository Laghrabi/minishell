/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:48:16 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/11 21:29:30 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	if_all_num(char *str)
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
	int				i;
	int				sign;
	unsigned long	result;

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
		if (result > (unsigned long)LONG_MAX + sign)
			return ((unsigned long)LONG_MAX + 1);
		i++;
	}
	if (sign)
		result *= -1;
	return ((long long)result);
}

static void	handle_exit_argument(t_token *token)
{
	unsigned long	nbr;

	nbr = ft_stoi(token->value);
	if (if_all_num(token->value) == 1)
	{
		ft_putstr_fd("minishell: numeric argument required\n", 2);
		s_var()->exit_status = 2;
	}
	else if (token->next != NULL)
	{
		ft_putstr_fd("minishell: too many arguments\n", 2);
		s_var()->exit_status = 1;
	}
	else if (nbr == (unsigned long)LONG_MAX + 1)
	{
		ft_putstr_fd("minishell: numeric argument required\n", 2);
		s_var()->exit_status = 2;
	}
	else
		s_var()->exit_status = nbr % 256;
}

int	builtin_exit(t_token *token)
{
	if (token == NULL)
		return (1);
	if (isatty(STDOUT_FILENO) && isatty(STDIN_FILENO))
		ft_putstr_fd("exit\n", 2);
	if (token->next != NULL)
	{
		token = token->next;
		handle_exit_argument(token);
		if (s_var()->exit_status == 1)
			return (1);
	}
	memory_management(*(s_var()->env_list), 1);
	exit(s_var()->exit_status);
}

// int	builtin_exit(t_token *token)
// {
// 	unsigned long	nbr;

// 	if (token == NULL)
// 		return (1);
// 	if (token->next == NULL)
// 		ft_putstr_fd("exit\n", 2);
// 	if (token->next != NULL)
// 	{
// 		token = token->next;
// 		nbr = ft_stoi(token->value);
// 		if (if_all_num(token->value) == 1)
// 		{
// 			ft_putstr_fd("exit\nminishell: numeric argument required\n", 2);
// 			s_var()->exit_status = 2;
// 		}
// 		else if (token->next != NULL)
// 		{
// 			ft_putstr_fd("exit\nminishell: too many arguments\n", 2);
// 			s_var()->exit_status = 1;
// 			return (1);
// 		}
// 		else if (nbr == (unsigned long)LONG_MAX + 1)
// 		{
// 			ft_putstr_fd("exit\nminishell: numeric argument required\n", 2);
// 			s_var()->exit_status = 2;
// 		}
// 		else
// 		{
// 			printf("exit\n");
// 			s_var()->exit_status = nbr % 256;
// 		}
// 	}
// 	exit(s_var()->exit_status);
// }
