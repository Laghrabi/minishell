/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:10:39 by claghrab          #+#    #+#             */
/*   Updated: 2025/05/07 15:31:35 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

//INCLUDES
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

//ENUMERATION
typedef enum s_type {
    T_WORD,
    T_PIPE,
    T_OR,
    T_AND,
    T_WILDCARD,
    T_DOUBLE_Q,
    T_SINGLE_Q,
    T_DOLLAR_S,
    T_RED_IN,
    T_RED_OUT,
    T_APPEND,
    T_HEREDOC,
    T_LPAREN,
    T_RPAREN 
} t_type;

//STRUCTURES
typedef struct s_token {
    char    *value;
    t_type  token;
    struct s_token *next;
} t_token;

#endif