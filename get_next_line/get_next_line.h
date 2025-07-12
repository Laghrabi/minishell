/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:11:27 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/12 20:31:44 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_listg
{
	char			*word;
	struct s_listg	*next;
}					t_listg;

char				*get_next_line(int fd);
char				*rest(t_listg *lst);
void				clear(t_listg **lst);
char				*squeezer(t_listg *lst);
int					size_lst(t_listg *lst);
int					checkk(t_listg *node);
t_listg				*new_node10(int fd, t_listg **the_line);
void				append(t_listg **lst, t_listg *node);

void				make(int fd, t_listg **the_line);

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 100
# endif

#endif