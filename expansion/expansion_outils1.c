/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_outils1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 20:39:58 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/12 22:28:01 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	skip_var(char *token, int *i)
{
	(*i)++;
	while (ft_isalnum(token[*i]) || token[*i] == '_')
		(*i)++;
	(*i)--;
}

void	write_value(t_extoken *ex_token, char *value)
{
	int		i;
	int		j;
	char	*new_token;
	int		*new_field;

	ex_token->buffer_size += ft_strlen(value);
	new_token = gc_malloc((ex_token->buffer_size + 1) * sizeof(char));
	new_field = gc_malloc((ex_token->buffer_size + 1) * sizeof(int));
	ft_bzero(new_field, (ex_token->buffer_size + 1) * sizeof(int));
	i = -1;
	while (++i < ex_token->index)
	{
		new_token[i] = ex_token->new_token[i];
		new_field[i] = ex_token->field[i];
	}
	j = 0;
	while (value[j])
	{
		new_token[i + j] = value[j];
		new_field[i + j] = ex_token->inherited_field | 1;
		j++;
	}
	ex_token->index = j + i;
	ex_token->new_token = new_token;
	ex_token->field = new_field;
}

int	*ft_subint(int *field, int start, int len)
{
	int	i;
	int	*new_field;

	if (!field)
		return (NULL);
	new_field = gc_malloc(len * sizeof(int));
	i = 0;
	while (i < len)
	{
		new_field[i] = field[start + i];
		i++;
	}
	return (new_field);
}

void	add_new_token(t_token **head, char *value, int *field, t_int db_int)
{
	char	*one_token;
	int		*token_field;
	t_token	*token;

	one_token = ft_substr(value, db_int.i, db_int.len - db_int.i);
	token_field = ft_subint(field, db_int.i, db_int.len - db_int.i);
	token = new_token(one_token, T_WORD, token_field);
	add_back(head, token);
}

int	i_o(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (1);
	return (0);
}
