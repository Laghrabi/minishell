/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 20:52:39 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/12 18:45:23 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	*syntax_error(int status)
{
	if (s_var()->printed == 0)
	{
		ft_putstr_fd("SYNTAX ERROR\n", 2);
		s_var()->printed = 1;
	}
	s_var()->exit_status = status;
	s_var()->syntax_error = status;
	return (NULL);
}

t_ast	*init_command_ast(void)
{
	return (create_ast_node(NULL, NULL, NULL, NODE_ARGS_LIST));
}

int	is_word_token(t_type type)
{
	return (type == T_WORD || type == T_DOLLAR_S
		|| type == T_SINGLE_Q || type == T_DOUBLE_Q
		|| type == T_WILDCARD);
}

t_ast	*parse_command(int *ctrc)
{
	t_ast	*node;

	if (peek() && peek()->token == T_LPAREN)
		return (parse_subshell(ctrc));
	else if (peek() && (peek()->token == T_WORD
			|| is_red_list(peek()->value) == 1))
	{
		node = parse_simple_command(ctrc);
		return (node);
	}
	else
		return (syntax_error(2));
}
