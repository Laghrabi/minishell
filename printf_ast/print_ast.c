/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 15:09:08 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/09 15:17:00 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_indent(int depth)
{
	int	i;

	i = 0;
	while (i < depth)
	{
		printf("  ");
		i++;
	}
}

void	print_node_type(t_node_type type)
{
	if (type == NODE_AND)
		printf("AND\n");
	else if (type == NODE_OR)
		printf("OR\n");
	else if (type == NODE_PIPE)
		printf("PIPE\n");
	else if (type == NODE_CMD)
		printf("CMD\n");
	else if (type == NODE_ARGS_LIST)
		printf("ARGS_LIST\n");
	else if (type == NODE_SUBSHELL)
		printf("SUBSHELL\n");
	else if (type == NODE_IREDIR)
		printf("REDIR_IN\n");
	else if (type == NODE_OREDIR)
		printf("REDIR_OUT\n");
	else if (type == NODE_APPEND)
		printf("REDIR_APPEND\n");
	else if (type == NODE_HEREDOC)
		printf("HEREDOC\n");
	else
		printf("UNKNOWN\n");
}

void	print_token_list(t_token *tokens, int depth)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		print_indent(depth);
		printf("TOKEN: [%s]\n", current->value);
		print_indent(depth);
		print_indent(depth);
		printf("\n");
		current = current->next;
	}
}

void	print_ast(t_ast *node, int depth)
{
	if (!node)
		return ;
	print_indent(depth);
	printf("Node Type: ");
	print_node_type(node->type);
	if (node->token_list)
	{
		print_indent(depth);
		printf("Token List:\n");
		print_token_list(node->token_list, depth + 1);
	}
	if (node->left)
	{
		print_indent(depth);
		printf("Left:\n");
		print_ast(node->left, depth + 1);
	}
	if (node->right)
	{
		print_indent(depth);
		printf("Right:\n");
		print_ast(node->right, depth + 1);
	}
}
