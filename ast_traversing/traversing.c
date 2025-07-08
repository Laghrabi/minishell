/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traversing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:01:47 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/08 22:46:52 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"



int	execute_ast(t_ast *node, t_env *env_list)
{
    if (!node)	
       return (EXIT_FAILURE);
    return execute_compound_command(node, env_list);
    return (EXIT_FAILURE);
}
