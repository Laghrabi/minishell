/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*    realloc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:31:12 by zfarouk           #+#    #+#             */
/*   Updated: 2025/05/27 18:41:13 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void *realloc(void *old_ptr, size_t size)
{
    void *ptr;
    
    ptr = malloc(size);
    ft_bzero(ptr, size);
    
}