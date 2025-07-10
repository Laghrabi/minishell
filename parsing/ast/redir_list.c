/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 21:05:23 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/10 00:20:55 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	redir_list_helper(t_ast **redir_head, t_ast **redir_tail)
{
	t_type(op_type);
	t_token(*file_name);
	t_ast(*redir);
	op_type = consume()->token;
	if (!peek() || peek()->token != T_WORD)
		return (1);
	file_name = consume();
	redir = create_ast_node(NULL, NULL, single_token_list(file_name),
			convert_t_type(op_type));
	if (*redir_head == NULL)
	{
		*redir_head = redir;
		*redir_tail = redir;
	}
	else
	{
		(*redir_tail)->right = redir;
		*redir_tail = redir;
	}
	return (0);
}

t_token	*single_token_list(t_token *token)
{
	if (token == NULL)
		return (NULL);
	else
	{
		token->pre = NULL;
		token->next = NULL;
	}
	return (token);
}

t_node_type	convert_t_type(t_type op)
{
	if (op == T_APPEND)
		return (NODE_APPEND);
	if (op == T_RED_IN)
		return (NODE_IREDIR);
	if (op == T_RED_OUT)
		return (NODE_OREDIR);
	if (op == T_HEREDOC)
		return (NODE_HEREDOC);
	return (0);
}

int	is_red_list(char *str)
{
	if (str == NULL)
		return (0);
	if (strncmp(str, ">>", 2) == 0)
		return (1);
	else if (strncmp(str, "<<", 2) == 0)
		return (1);
	else if (strncmp(str, ">", 1) == 0)
		return (1);
	else if (strncmp(str, "<", 1) == 0)
		return (1);
	else
		return (0);
}

t_ast	*parse_redir_list(void)
{
	t_type(op_type);
	t_token(*file_name);
	t_ast(*redir), (*redir_head), (*redir_tail);
	redir_head = NULL;
	redir_tail = NULL;
	while (peek() && is_red_list(peek()->value) == 1)
	{
		op_type = consume()->token;
		if (!peek() || peek()->token != T_WORD)
			return (syntax_error(2));
		file_name = consume();
		redir = create_ast_node(NULL, NULL, single_token_list(file_name),
				convert_t_type(op_type));
		if (redir_head == NULL)
		{
			redir_head = redir;
			redir_tail = redir;
		}
		else
		{
			redir_tail->right = redir;
			redir_tail = redir;
		}
	}
	return (redir_head);
}
