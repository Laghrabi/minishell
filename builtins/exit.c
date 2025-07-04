/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:48:16 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/04 22:56:23 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int if_all_num(char *str)
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
	int		i;
	int		sign;
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

int	builtin_exit(t_token *token)
{
	unsigned long	nbr;

	if (token == NULL)
		return (1);
	if (token->next == NULL)
	{
		if (isatty(0))
			printf("exit\n");
		//exit(0);
	}
	else if (token->next != NULL)
	{
		token = token->next;
		if (token->next != NULL)
		{
			printf("exit\nbash: exit: too many arguments\n");
			s_var()->exit_status = 1;
			return (1);
		}
		if (if_all_num(token->value) == 1)
			return (1);
		nbr = ft_stoi(token->value);
		if (nbr == (unsigned long)LONG_MAX + 1)
		{
			printf("exit\nbash: exit: %s: numeric argument required\n", token->value);
			//exit(2);
			s_var()->exit_status = 2;
		}
		else
		{
			printf("exit\n");
			s_var()->exit_status = nbr % 256;
			//exit(nbr % 256);
		}
	}
	printf("EXIT STATUS: [%d]", s_var()->exit_status);
	exit(s_var()->exit_status);
	//return (1);
}
