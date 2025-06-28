/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 04:21:58 by zfarouk           #+#    #+#             */
/*   Updated: 2025/06/27 04:28:46 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

typedef struct s_variabls
{
    t_env	*env_list;
    int     exit_status;
} t_variabls;


t_variabls *g_var (void)
{
    static t_variabls global = {0};

    return (&global);
}
