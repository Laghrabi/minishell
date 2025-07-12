/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_signals.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 21:17:08 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/12 01:32:19 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	sigint_handler_child(int signum)
{
	(void)signum;
	write(STDOUT_FILENO, "\n", 1);
	close(s_var()->fd);
	memory_management(*(s_var()->env_list), 1);
	exit(130);
}
