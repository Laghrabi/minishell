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

    while (1)
    {
        input = readline("minishell$ ");
        printf("%s\n", input);
        free(input);
    }

    return 0;
}