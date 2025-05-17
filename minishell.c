/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:39:50 by zfarouk           #+#    #+#             */
/*   Updated: 2025/05/16 18:06:19 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *g_token;

int main()
{
    char *input;
    t_ast *ast;
    //t_token *token;
    while (1)
    {
        input = readline("minishell$ ");
        //printf("%s\n", input);
        g_token = tokenization(input);
        free(input);
        t_token *current = g_token;
        while (current)
        {
            printf("[%s]  [%d]\n", current->value, current->token);
            current = current->next;
            printf("\n");
        }
        ast = parse_compound_command();
    }

    return 0;
}