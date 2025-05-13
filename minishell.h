/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:10:39 by claghrab          #+#    #+#             */
/*   Updated: 2025/05/13 20:49:45 by claghrab         ###   ########.fr       */
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
# include "libft/libft.h"

t_token	*g_token;

/* ENUMERATION */
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

typedef enum e_node_type {
    NODE_CMD,
    NODE_PIPE,
    NODE_AND,
    NODE_OR,
    NODE_REDIR,
    NODE_SUBSHELL
} t_node_type;

/* STRUCTURES */
typedef struct s_ast {
    t_type          type;
    t_token         *token_list;    // for NODE_CMD
    struct s_ast    *left;
    struct s_ast    *right;
} t_ast;

typedef struct s_token {
    t_type          token;
    char            *value;
    struct s_token  *next;
} t_token;

typedef struct s_info {
    char *str;
    int size;
    t_type type;
    void		(*func)(char *input,struct s_info info, int *i, t_token **token);
} t_info;


/* FUNCTIONS */
int	is_red_list(char *str);
t_token	*peek(void);
t_token	*consume(void);
void	*syntax_error(void);
t_ast	*create_ast_node(t_ast *left, t_ast *right, t_token	*token_list, t_node_type type);
t_token *tokenization(char *input);
void word_sep(char *input, t_info info, int *i, t_token **token);
void operator_sep(char *input, t_info info, int *i, t_token **token);

#endif