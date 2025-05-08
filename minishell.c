/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk  <zfarouk@student.1337.ma>         #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-07 14:39:50 by zfarouk           #+#    #+#             */
/*   Updated: 2025-05-07 14:39:50 by zfarouk          ###   ########.ma       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main()
{
    char *input;
    t_token *token;
    while (1)
    {
        input = readline("minishell$ ");
        //printf("%s\n", input);
        token = tokenization(input);
        free(input);
        t_token *current = token;
        while (current)
        {
            printf("[%s]  ", current->value);
            current = current->next;
            printf("\n");
        }
    }

    return 0;
}