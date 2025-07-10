/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 20:52:39 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/10 19:00:48 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_ast	*create_ast_node(t_ast *left, t_ast *right, t_token *token_list,
		t_node_type type)
{
	t_ast	*node;

	node = gc_malloc(sizeof(t_ast));
	if (node == NULL)
		return (NULL);
	node->left = left;
	node->right = right;
	node->token_list = token_list;
	node->type = type;
	return (node);
}

t_ast	*parse_pipeline(int *ctrc)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*node;

	left = parse_command(ctrc);
	if (left == NULL)
		return (NULL);
	while (peek() && peek()->token == T_PIPE)
	{
		consume();
		right = parse_pipeline(ctrc);
		if (right == NULL)
			return (NULL);
		node = create_ast_node(left, right, NULL, NODE_PIPE);
		if (node == NULL)
			return (NULL);
		left = node;
	}
	return (left);
}
