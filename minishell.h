/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:10:39 by claghrab          #+#    #+#             */
/*   Updated: 2025/05/31 15:18:43 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

//INCLUDES
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"

#define double_quoted = 4;
#define single_quoted = 2;
#define expanded = 1;
#define normale = 0;

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
    NODE_ARGS_LIST,
    NODE_REDIR,
    NODE_IREDIR,
    NODE_OREDIR,
    NODE_APPEND,
    NODE_HEREDOC,
    NODE_SUBSHELL
} t_node_type;

/* STRUCTURES */
typedef struct s_token {
    t_type          token;
    char            *value;
    struct s_token  *next;
    struct s_token  *pre;
} t_token;

extern t_token	*g_token;

typedef struct s_ast {
    //t_type          type;
    t_node_type     type;
    t_token         *token_list;    // for NODE_CMD
    struct s_ast    *left;
    struct s_ast    *right;
} t_ast;

typedef struct s_env {
    char			*key;
	char			*value;
	struct s_env	*next;
} t_env;

typedef struct double_int {
    int i;
    int len;
} db_int;

typedef struct expanded_token
{
    char    *new_token;
    int     buffer_size;
    int     index;
    int     removed_quote;
} t_extoken;

typedef struct s_info {
    char *str;
    int size;
    t_type type;
    int		(*func)(char *input,struct s_info info, int *i, t_token **token);
} t_info;


typedef struct s_gr_cl
{
    void *pointer;
    struct s_gr_cl *next;
    struct s_gr_cl *prev;
} t_gr_cl;


/* FUNCTIONS */
int which_type(char c, int *db_quote, int *sg_quote, int *quote_state);
int *set_field(char *str);
void skip_var(char *token, int *i);
int extra_quote(char c, int i);
char *check_var(char *token, t_env *env, int *index);
void fix_value(char *value);
char *expanded_token(char *token, int *field, t_env *env);
void expansion(t_token *arg_list, t_env *env);
void	update_env(char *key, char *new_value, t_env *env_list);
char    *get_env_value(char *key, t_env *env_list);
t_env	*init_env(char **envp);
void	env_add_back(t_env **lst, t_env *new);
t_env	*find_last(t_env *lst);
int	find_chr_pos(char *str, char c);
t_gr_cl *last_node(t_gr_cl *gar);
t_gr_cl *new_node(void *ptr);
void free_garbage_collector(t_gr_cl *garbage);
void garbage_collector(void *new_pointer, int  should_free);
void *gc_malloc(size_t size);
int	is_red_list(char *str);
t_token	*peek(void);
t_token	*consume(void);
void	*syntax_error(void);
t_ast   *parse_simple_command(void);
t_ast   *parse_subshell(void);
t_ast   *parse_command(void);
t_ast   *parse_pipeline(void);
t_ast   *parse_compound_command(bool subshell);
t_ast   *parse_redir_list(void);
t_node_type	convert_t_type(t_type op);
void	append_token(t_token **head, t_token *new_token);
t_token	*single_token_list(t_token *token);
t_ast	*create_ast_node(t_ast *left, t_ast *right, t_token	*token_list, t_node_type type);
t_token *tokenization(char *input);
int word_sep(char *input, t_info info, int *i, t_token **token);
int operator_sep(char *input, t_info info, int *i, t_token **token);

#endif