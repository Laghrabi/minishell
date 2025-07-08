/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:49:21 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/08 21:31:48 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	operator(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '(' || c == ')' || c == ' ')
		return (1);
	return (0);
}

void	is_qoute(char c, int *i, int *j)
{
	if (c == '\'' && *j == 0 && *i == 0)
		*j = 1;
	else if (c == '\'' && *j == 1)
		*j = 0;
	else if (c == '\"' && *i == 0 && *j == 0)
		*i = 1;
	else if (c == '\"' && *i == 1)
		*i = 0;
}

void	init_struct(t_info **info)
{
	static t_info	all[] = {
	{"||",	2,	T_OR,	operator_sep},
	{"|",	1,	T_PIPE,	operator_sep},
	{"&&",	2,	T_AND,	operator_sep},
	{">>",	2,	T_APPEND,	operator_sep},
	{"<<",	2,	T_HEREDOC,	operator_sep},
	{"<",	1,	T_RED_IN,	operator_sep},
	{">",	1, T_RED_OUT, operator_sep},
	{"(", 1, T_LPAREN, operator_sep},
	{")", 1, T_RPAREN, operator_sep},
	{"", 0, T_WORD, word_sep}};

	*info = all;
}

t_token	*tokenization(char *input, int i, int check, t_token *token)
{
	int		j;
	t_info	*info;

	info = NULL;
	init_struct(&info);
	while (input[i])
	{
		j = -1;
		while (input[i] == ' ' || input[i] == '\t')
			i++;
		while (input[i] && ++j < 10)
		{
			if (ft_strncmp(&input[i], info[j].str, info[j].size) == 0)
			{
				check = info[j].func(input + i, info[j], &i, &token);
				break ;
			}
			if (check == 0)
			{
				printf("quote not closed\n");
				return (NULL);
			}
		}
	}
	return (token);
}
