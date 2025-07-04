/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:10:39 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/06 01:19:22 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

//INCLUDES
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdbool.h>
# include <aio.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"
# include <dirent.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>


#define LONG_LONG_MAX 9223372036854775807LL
// #define LONG_MAX 9223372036854775807L
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
    int             ambiguous;
    int             is_herdoc;
    int             expansion;
    int             *field;
    int             empty;
    int             is_already_exec;
} t_token;

extern t_token	*g_token;

typedef struct s_ast {
    t_node_type     type;
    t_token         *token_list;
    struct s_ast    *left;
    struct s_ast    *right;
} t_ast;

typedef struct s_env {
    char			*key;
	char			*value;
	struct s_env	*next;
    struct s_env	*export_next;
} t_env;

typedef struct double_int {
    int i;
    int len;
} db_int;

typedef struct expanded_token
{
    char    *new_token;
    int     *field;
    int     buffer_size;
    int     index;
    int     inherited_field;
    int     ambiguous;
    int     empty;
} t_extoken;

typedef struct s_info {
    char *str;
    int size;
    t_type type;
    int		(*func)(char *input,struct s_info info, int *i, t_token **token);
} t_info;

typedef struct s_variabls
{
    t_env	*env_list;
    int     exit_status;
    int     g_child_ctrc;
    int     syntax_error;
} t_variabls;

typedef struct s_gr_cl
{
    void *pointer;
    struct s_gr_cl *next;
    struct s_gr_cl *prev;
} t_gr_cl;

/* FUNCTIONS */
void	child_process(int pipefd[2], char *delimiter);
int	parent_process(pid_t pid, int pipefd[2], int *ctrc);
char	*read_from_pipe(int pipefd);
char	*join(char *s1, char *s2);
t_ast *init_command_ast(void);
int is_word_token(t_type type);
int	redir_list_helper(t_ast **redir_head, t_ast **redir_tail);
void    sigint_handler_child(int signum);
char	*read_heredoc_lines(char *delimiter, int *ctrc);
char	*parse_herdoc_helper(int *i, int *ctrc);
int	parse_herdoc(t_ast **redir_head, t_ast **redir_tail, int *ctrc);
void	setup_signals(void);
int	check_for_var(char *key, t_env *env_list);
int is_di_or_builtin(t_ast *node, t_env *env_list, int i);
int is_absolute_path(t_ast *node, t_env *env_lsit);
int is_dir(t_ast *node, t_env *env_list);
int is_executable(t_ast *node, t_env *env_list);
int is_path(t_ast *node, t_env *env_list);
char	*ft_strdup2(const char *s1);
char	*ft_substr2(char const *s, unsigned int start, size_t len);
void memory_management(t_env *env, int free_env);
char	*ft_substr2(char const *s, unsigned int start, size_t len);
void expand_evrything(t_ast *node, t_env *env_list);
t_variabls *s_var (void);
int execute_compound_command(t_ast *node, t_env *env_list);
char **convert_env_to_array(t_env *env_list);
char	*find_cmd_path(char *cmd, t_env *env_list);
int execute_pipe(t_ast *node, t_env *env_list, int input_fd);
int handle_simple_command(t_ast *node, t_env *env_list);
int	setup_redirections(t_ast *redir_list, t_env *env_list);
int	check_nm_var(char *str);
char **token_list_to_argv(t_token *token_list);
int	check_for_var(char *key, t_env *env_list);
int	if_builtin(char *cmd);
int	which_one(int flag, t_token *token, t_env *env_list);
int	cmd_or_builtin(t_token *token, t_env *env_list, char **argv);
int	builtin_exit(t_token *token);
long long	ft_stoi(const char *str);
int if_all_num(char *str);
int	builtin_export(t_token *token, t_env **env_list);
int	builtin_unset(t_token *token, t_env **env_list);
int	builtin_env(t_token *token, t_env *env_list);
int	builtin_pwd(t_token *token, t_env *env_list);
int	builtin_cd(t_token *token, t_env *env_list);
int	cd_helper(char *new_path, t_env *env_list);
int	builtin_echo(t_token *token, t_env *env_list);
int execute_command(t_ast *node, t_env *env_list);
int	execute_ast(t_ast *node, t_env *env_list);
void wildcard(t_token **arg_list);
void    add_back(t_token **token, t_token *new);
int *set_field(char *str);
void skip_var(char *token, int *i);
int extra_quote(char c, int i);
char *check_var(char *token, t_env *env, int *index);
void fix_value(char *value, t_token **arg);
t_extoken *expanded_token(char *token, t_env *env, int k);
void expansion(t_token **arg_list, t_env *env);
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
void	*syntax_error(int status);
t_ast   *parse_simple_command(int *ctrc);
t_ast   *parse_subshell(int *ctrc);
t_ast   *parse_command(int *ctrc);
t_ast   *parse_pipeline(int *ctrc);
t_ast   *parse_compound_command(bool subshell, int *ctrc);
t_ast   *parse_redir_list(void);
t_node_type	convert_t_type(t_type op);
void	append_token(t_token **head, t_token *new_token);
t_token	*single_token_list(t_token *token);
t_ast	*create_ast_node(t_ast *left, t_ast *right, t_token	*token_list, t_node_type type);
t_token    *new_token(char *str, t_type type, int *field);
t_token *tokenization(char *input);
int word_sep(char *input, t_info info, int *i, t_token **token);
int operator_sep(char *input, t_info info, int *i, t_token **token);

#endif
