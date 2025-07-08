/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_handle_quote.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 20:46:41 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/08 20:51:00 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_single_quote(int *db_quote, int *sg_quote, int *quote_state)
{
	if (*db_quote == 1)
		return (*quote_state);
	else if (*sg_quote == 1)
	{
		*sg_quote = 0;
		*quote_state = 0;
	}
	else if (*sg_quote == 0)
	{
		*sg_quote = 1;
		*quote_state = 2;
		return (0);
	}
	return (*quote_state);
}

int	handle_double_quote(int *db_quote, int *sg_quote, int *quote_state)
{
	if (*sg_quote == 1)
		return (*quote_state);
	if (*db_quote == 1)
	{
		*db_quote = 0;
		*quote_state = 0;
	}
	else if (*db_quote == 0)
	{
		*db_quote = 1;
		*quote_state = 4;
		return (0);
	}
	return (*quote_state);
}

int	which_type(char c, int *db_quote, int *sg_quote, int *quote_state)
{
	if (c == '\'')
		return (handle_single_quote(db_quote, sg_quote, quote_state));
	else if (c == '\"')
		return (handle_double_quote(db_quote, sg_quote, quote_state));
	return (*quote_state);
}
