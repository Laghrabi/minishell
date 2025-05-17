/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:31:39 by claghrab          #+#    #+#             */
/*   Updated: 2025/05/17 11:46:46 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_ast   *parse_compound_command(void)
{
    t_type      op_type;
    t_node_type node_type;

    t_ast (*left), (*right), (*node);
    left = parse_pipeline();
    if (left == NULL)
        return (NULL);
    while (peek() && (peek()->token == T_AND || peek()->token == T_OR))
    {
        op_type = consume()->token;
        if (op_type == T_AND)
            node_type = NODE_AND;
        else
            node_type = NODE_OR;
        right = parse_compound_command();
        if (right == NULL)
            return (NULL);
        node = create_ast_node(left, right, NULL, node_type);
        if (node == NULL)
            return (NULL);
        left = node;
    }
    if (peek() && (peek()->token!= T_AND || peek()->token != T_OR))
        return (syntax_error());
    return (left);
}

t_ast   *parse_pipeline(void)
{
    t_ast   *left;
    t_ast   *right;
    t_ast   *node;

    left = parse_command();
    if (left == NULL)
        return (NULL);
    while (peek() && peek()->token == T_PIPE)
    {
        consume();
        right = parse_pipeline();
        if (right == NULL)
            return (NULL);
        node = create_ast_node(left, right, NULL, NODE_PIPE);
        if (node == NULL)
            return (NULL);
        left = node;
    }
    return (left);
}

t_ast   *parse_command(void)
{
    if (peek() == NULL)
        return (syntax_error());
    if (peek()->token == T_LPAREN)
        return (parse_subshell());
    else if (peek()->token == T_WORD || is_red_list(peek()->value) == 1)
        return (parse_simple_command());
    else
        return (syntax_error());
}

t_ast   *parse_subshell(void)
{
    t_ast   *inner_command;
    t_ast   *red_list;
    t_ast   *node;

    red_list = NULL;
    if (peek() == NULL || peek()->token != T_LPAREN)
        return (syntax_error());
    consume();
    inner_command = parse_compound_command();
    printf("here1\n");
    if (peek() == NULL || peek()->token != T_RPAREN)
        return (syntax_error());
    consume();
    if (peek() && is_red_list(peek()->value) == 0)
        red_list = parse_redir_list();
    node = create_ast_node(inner_command, red_list, NULL, NODE_SUBSHELL);
    if (node == NULL)
        return (NULL);
    return (node);
}

t_ast   *parse_simple_command(void)
{
    t_ast   (*cmd), (*args_node),(*redir_head);
    args_node = create_ast_node(NULL, NULL, NULL, NODE_ARGS_LIST);
    redir_head = NULL;
    while (peek() && (peek()->token == T_WORD || peek()->token == T_DOLLAR_S || peek()->token == T_SINGLE_Q || peek()->token == T_DOUBLE_Q || peek()->token == T_WILDCARD))
        append_token(&args_node->token_list, consume());
    if (peek() && is_red_list(peek()->value) == 1)
        redir_head = parse_redir_list();
    if (!args_node->token_list && !redir_head)
        return (syntax_error());
    printf("here2\n");
    cmd = create_ast_node(args_node, redir_head, NULL, NODE_CMD);
    return (cmd);
}

t_ast   *parse_redir_list(void)
{
    t_type  (op_type);
    t_token (*file_name);
    t_ast   (*redir),(*redir_head), (*redir_tail);
    redir_head = NULL;
    redir_tail = NULL;
    while (peek() && is_red_list(peek()->value) == 1)
    {
        op_type = consume()->token;
        if (!peek() || (peek()->token != T_WORD && peek()->token != T_DOLLAR_S && peek()->token != T_SINGLE_Q && peek()->token != T_DOUBLE_Q))
            return (syntax_error());
        file_name = consume();
        redir = create_ast_node(NULL, NULL, single_token_list(file_name), convert_t_type(op_type));
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
