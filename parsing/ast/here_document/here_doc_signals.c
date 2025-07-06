/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_signals.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 21:17:08 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/05 21:19:53 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	sigint_handler_child(int signum)
{
    (void)signum;
    write(STDOUT_FILENO, "\n", 1);
    exit(130);
}
