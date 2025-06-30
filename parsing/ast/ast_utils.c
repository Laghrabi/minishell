/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:01:37 by claghrab          #+#    #+#             */
/*   Updated: 2025/06/30 23:00:45 by claghrab         ###   ########.fr       */
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

void	*syntax_error(long	status)
{
	printf("SYNTAX ERROR\n");
	s_var()->exit_status = status;
	return (NULL);
}

t_ast	*create_ast_node(t_ast *left, t_ast *right, t_token	*token_list, t_node_type type)
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
	//printf("new_token: %s\n", new_token->value);
	new_token->next = NULL;
	if (*head == NULL)
	{
		*head = new_token;
		(*head)->pre = NULL;
		return ;
	}
	tmp = *head;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new_token;
	new_token->next = NULL;
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
