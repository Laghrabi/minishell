/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_signals.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 21:17:08 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/08 15:17:34 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	sigint_handler_child(int signum)
{
	(void)signum;
	write(STDOUT_FILENO, "\n", 1);
	close(s_var()->pipe[0]);
	close(s_var()->pipe[1]);
	exit(130);
}
