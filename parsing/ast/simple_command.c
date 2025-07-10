/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 20:52:39 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/05 20:56:54 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	handle_word_token(t_ast *args_node)
{
	if (peek() && is_word_token(peek()->token))
	{
		append_token(&args_node->token_list, consume());
		return (0);
	}
	return (1);
}

static int	handle_heredoc(t_ast **redir_head, t_ast **redir_tail, int *ctrc)
{
	if (*ctrc == 0)
	{
		if (parse_herdoc(redir_head, redir_tail, ctrc) == 1)
			return (1);
	}
	else
	{
		consume();
		consume();
	}
	return (0);
}

static int	handle_redirection(t_ast **redir_head, t_ast **redir_tail,
		int *ctrc)
{
	if (peek()->token == T_HEREDOC)
	{
		if (handle_heredoc(redir_head, redir_tail, ctrc) == 1)
			return (1);
	}
	else if (redir_list_helper(redir_head, redir_tail) == 1)
		return (1);
	return (0);
}

static int	validate_command(t_ast *args_node, t_ast *redir_head, int ctrc)
{
	if (!args_node->token_list && !redir_head && ctrc == 0)
		return (1);
	return (0);
}

t_ast	*parse_simple_command(int *ctrc)
{
	t_ast(*args_node), (*redir_head), (*redir_tail);
	args_node = init_command_ast();
	redir_head = NULL;
	redir_tail = NULL;
	while (peek())
	{
		if (handle_word_token(args_node) == 0)
			continue ;
		if (peek() && is_red_list(peek()->value) == 1)
		{
			if (handle_redirection(&redir_head, &redir_tail, ctrc) == 1)
				return (syntax_error(2));
			continue ;
		}
		break ;
	}
	if (validate_command(args_node, redir_head, *ctrc) == 1)
		return (syntax_error(2));
	return (create_ast_node(args_node, redir_head, NULL, NODE_CMD));
}
