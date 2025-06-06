/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:49:21 by zfarouk           #+#    #+#             */
/*   Updated: 2025/05/27 17:21:06 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void init_struct(t_info **info)
{
    static t_info all [] = {
        {"||", 2, T_OR, operator_sep},
        {"|", 1, T_PIPE, operator_sep},
        {"&&", 2, T_AND, operator_sep},
        {"*", 1, T_WILDCARD, word_sep},
        {"\"", 1, T_DOUBLE_Q, word_sep},
        {"'", 1, T_SINGLE_Q, word_sep},
        {"$", 1, T_DOLLAR_S, word_sep},
        {">>", 2, T_APPEND, operator_sep},
        {"<<", 2, T_HEREDOC, operator_sep},
        {"<", 1, T_RED_IN, operator_sep},
        {">", 1, T_RED_OUT, operator_sep},
        {"(", 1, T_LPAREN, operator_sep},
        {")", 1, T_RPAREN, operator_sep},
        {"", 0, T_WORD, word_sep}
    };
    *info = all;
}


t_token *tokenization(char *input)
{
    int i;
    int j;
    t_info *info;
    t_token *token;
    int check;

    token = NULL;
    info = NULL;
    check = 1;
    init_struct(&info);
    i = 0;
    while (input[i])
    {
        j = -1;
        while (input[i] == ' ' || input[i] == '\t')
            i++;
        while (++j < 14)
        {
            if (ft_strncmp(&input[i], info[j].str, info[j].size) == 0)
            {
                check = info[j].func(input + i, info[j], &i, &token);
                break;
            }
            if (check == 0)
            {
                printf("quote not closed\n");
                return (NULL);
            }
        }
    }
    return token;
}
