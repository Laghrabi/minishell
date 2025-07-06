/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 20:52:39 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/05 21:10:14 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	append_token(t_token **head, t_token *new_token)
{
	t_token	*tmp;
	
	if (head == NULL || new_token == NULL)
		return ;
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

t_ast   *parse_subshell(int *ctrc)
{
    t_ast   *inner_command;
    t_ast   *red_list;
    t_ast   *node;

    red_list = NULL;
    if (peek() == NULL || peek()->token != T_LPAREN)
        return (syntax_error(2));
    consume();
    inner_command = parse_compound_command(true, ctrc);
    if (peek() == NULL || peek()->token != T_RPAREN)
        return (syntax_error(2));
    consume();
    if (peek() && is_red_list(peek()->value) == 1)
        red_list = parse_redir_list();
    node = create_ast_node(inner_command, red_list, NULL, NODE_SUBSHELL);
    if (node == NULL)
        return (NULL);
    return (node);
}
