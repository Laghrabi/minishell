/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compound_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 20:52:39 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/10 18:56:43 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_token	*peek(void)
{
	return (g_token);
}

t_token	*consume(void)
{
	t_token	*tmp;

	tmp = g_token;
	if (g_token)
		g_token = g_token->next;
	return (tmp);
}

int	long_line(bool subshell)
{
	if ((peek()->token == T_RPAREN && subshell == false)
		|| (peek()->token != T_AND && peek()->token != T_OR
			&& subshell == false))
		return (1);
	return (0);
}

t_node_type	assign_type(t_type op_type)
{
	if (op_type == T_AND)
		return (NODE_AND);
	else
		return(NODE_OR);
}

t_ast	*parse_compound_command(bool subshell, int *ctrc)
{
	t_type (op_type);
	t_node_type (node_type);
	t_ast (*right);
	t_ast (*left);
	t_ast (*node);
	left = parse_pipeline(ctrc);
	if (left == NULL)
		return (NULL);
	while (peek() && (peek()->token == T_AND || peek()->token == T_OR))
	{
		op_type = consume()->token;
		node_type = assign_type(op_type);
		right = parse_compound_command(subshell, ctrc);
		if (right == NULL)
			return (NULL);
		node = create_ast_node(left, right, NULL, node_type);
		if (node == NULL)
			return (NULL);
		left = node;
	}
	if (peek() && long_line(subshell) == 1)
		return (syntax_error(2));
	return (left);
}
