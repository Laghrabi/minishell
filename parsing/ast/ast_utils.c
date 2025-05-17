/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:01:37 by claghrab          #+#    #+#             */
/*   Updated: 2025/05/16 17:25:03 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_token	*peek(void)
{
	return (g_token);
}

t_token	*consume(void)
{
	t_token *tmp;
	
	tmp = g_token;
	if (g_token)
		g_token = g_token->next;
	return (tmp);
}

void	*syntax_error(void)
{
	printf("SYNTAX ERROR\n");
	return (NULL);
}

t_ast	*create_ast_node(t_ast *left, t_ast *right, t_token	*token_list, t_node_type type)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (node == NULL)
		return (NULL);
	node->left = left;
	node->right = right;
	node->token_list = token_list;
	node->type = type;
	return (node);
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

void	append_token(t_token **head, t_token *new_token)
{
	t_token	*tmp;
	
	if (head == NULL || new_token == NULL)
		return ;
	new_token->next = NULL;
	if (*head == NULL)
	{
		*head = new_token;
		return ;
	}
	tmp = *head;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new_token;
}

t_token	*single_token_list(t_token *token)
{
	if (token == NULL)
		return (NULL);
	else
		token->next = NULL;
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
