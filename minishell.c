/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:39:50 by zfarouk           #+#    #+#             */
/*   Updated: 2025/05/19 13:17:05 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *g_token;


#include <stdio.h>

void print_indent(int depth)
{
    for (int i = 0; i < depth; ++i)
        printf("  ");  // two spaces per depth level
}

void print_node_type(t_node_type type)
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

void print_token_list(t_token *tokens, int depth)
{
    t_token *current = tokens;
    while (current)
    {
        print_indent(depth);
        printf("TOKEN: [%s]\n", current->value);
        current = current->next;
    }
}

void print_ast(t_ast *node, int depth)
{
    if (!node)
        return;

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


int main()
{
    char *input;
    t_ast *ast;
    //t_token *token;
    while (1)
    {
        input = readline("minishell$ ");
        add_history(input);
        //printf("%s\n", input);
        g_token = tokenization(input);
        if (g_token == NULL)
            continue;
        free(input);
        t_token *current = g_token;
        while (current)
        {
            printf("[%s]  [%d]\n", current->value, current->token);
            current = current->next;
            printf("\n");
        }
        if (peek())
            ast = parse_compound_command(false);
        if (ast)
            print_ast(ast, 0);
        garbage_collector(NULL, 1);
    }

    return 0;
}