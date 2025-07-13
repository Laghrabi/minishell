/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:10:39 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/13 01:38:12 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

//INCLUDES
# include "get_next_line/get_next_line.h"
# include "libft/libft.h"
# include <aio.h>
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define LONG_LONG_MAX 9223372036854775807LL

/* ENUMERATION */
typedef enum s_type
{
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
}						t_type;

typedef enum e_node_type
{
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
}						t_node_type;

/* STRUCTURES */
typedef struct s_token
{
	t_type				token;
	char				*value;
	struct s_token		*next;
	struct s_token		*pre;
	int					ambiguous;
	int					is_herdoc;
	int					expansion;
	int					*field;
	int					empty;
	int					is_already_exec;
}						t_token;

extern t_token			*g_token;

typedef struct s_ast
{
	t_node_type			type;
	t_token				*token_list;
	struct s_ast		*left;
	struct s_ast		*right;
}						t_ast;

typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
	struct s_env		*prev;
}						t_env;

typedef struct double_int
{
	int					i;
	int					len;
}						t_int;

typedef struct double_par
{
	t_ast				*node;
	t_env				*env_list;
}						t_par;

typedef struct expanded_token
{
	char				*new_token;
	int					*field;
	int					buffer_size;
	int					index;
	int					inherited_field;
	int					ambiguous;
	int					empty;
}						t_extoken;

typedef struct s_info
{
	char				*str;
	int					size;
	t_type				type;
	int					(*func)(char *input, struct s_info info, int *i,
			t_token **token);
}						t_info;

typedef struct s_variabls
{
	t_env				**env_list;
	int					fd;
	int					exit_status;
	int					g_child_ctrc;
	int					syntax_error;
	int					printed;
	int					pipe[2];
}						t_variabls;

typedef struct s_gr_cl
{
	void				*pointer;
	struct s_gr_cl		*next;
	struct s_gr_cl		*prev;
}						t_gr_cl;

typedef struct s_pipes
{
	int					pipe_a[2];
	int					pipe_b[2];
	int					*cur_pipe;
	int					*prev_pipe;
	int					*tmp;
}						t_pipe;

typedef struct s_mul
{
	int					*pids;
	int					*cmd_count;
	int					*input_fd;
}						t_mul;

typedef struct s_unliked
{
	char				*file_name;
	struct s_unliked	*next;
}						t_unlinked;

/* FUNCTIONS */

/*print ast*/
void					print_indent(int depth);
void					print_node_type(t_node_type type);
void					print_token_list(t_token *tokens, int depth);
void					print_ast(t_ast *node, int depth);

/*wildcard*/
void					wildcard(t_token **arg_list);
void					check_wildcard(t_token **token);
void					append_matches(t_token **token, t_token *match);
t_token					*search_matches(char *str);
int						is_matche(char *str, char *suspect);

/*memory management*/
void					free_env(t_env *env);
void					memory_management(t_env *env, int is_free_env);
t_gr_cl					*last_node(t_gr_cl *gar);
t_gr_cl					*new_node(void *ptr);
void					free_garbage_collector(t_gr_cl *garbage);
void					garbage_collector(void *new_pointer, int should_free);
void					*gc_malloc(size_t size);

/*envirement list*/
char					*ft_strdup2(const char *s1);
int						find_chr_pos(char *str, char c);
t_env					*find_last(t_env *lst);
void					env_add_back(t_env **lst, t_env *new);
void					increment_shelvl_value(t_env *envp);
t_env					*init_env(char **envp);
char					*ft_substr2(char const *s, unsigned int start,
							size_t len);
void					copy_env_key_value(char **argv, int *i, t_env *env);
char					**convert_env_to_array(t_env *env_list);

/*execution function*/
int						num_of_pipes(t_ast *node);
void					replace_last_executed_cmd(t_env *env_list, char **argv);
void					free_double_array(char **db_str);
int						handle_wait_and_status(int pid[1024], int cmd_count);
t_pipe					*initialize_pipe(void);
int						fork_child_for_pipe(t_ast *node, t_env *env_list,
							int input_fd, int pipefd[2]);
int						handle_simple_command(t_ast *node, t_env *env_list,
							char **argv);
int						execute_simple_cmd(t_env *env_list, char **argv);
char					**token_list_to_argv(t_token *token_list);
int						is_dir1(t_ast *node, t_env *env_list);
char					*ft_strjoin2(char const *s1, char const *s2);
char					**path_splitting(t_env *envp);
char					*find_cmd_path(char *cmd, t_env *env_list);
char					*get_file_name(t_ast *redir_node);
int						open_herdoc_file(char *text, int expans);
char					*creat_herdoc_file(void);
int						heredoc_to_fd(t_ast *heredoc_node, t_env *env_list);
int						handle_one_redirection(t_ast *redir, t_env *env_list);
int						setup_redirections(t_ast *redir_list, t_env *env_list);
int						is_executable(t_ast *node, t_env *env_list);
int						is_dir(t_ast *node, t_env *env_list);
int						is_absolute_path(t_ast *node, t_env *env_list);
int						is_di_or_builtin(t_ast *node, t_env *env_list, int i);
int						is_path(t_ast *node, t_env *env_list);
void					fd_leaks(int fd1, int fd2);
int						execute_subshell(t_ast *node, t_env *env_list);
int						handle_simple_command(t_ast *node, t_env *env_list,
							char **argv);
int						execute_command(t_ast *node, t_env *env_list);
int						handle_wait_and_status(int pid[1024], int cmd_count);
t_pipe					*initialize_pipe(void);
int						fork_child_for_pipe(t_ast *node, t_env *env_list,
							int input_fd, int pipefd[2]);
int						execute_last_command(t_ast *node, t_env *env_list,
							int pipe_read_end);
void					expand_evrything(t_ast *node, t_env *env_list);
int						execute_pipe(t_ast *node, t_env *env_list,
							int input_fd);
int						handle_node_and(t_ast *node, t_env *env_list);
int						handle_node_or(t_ast *node, t_env *env_list);
int						execute_compound_command(t_ast *node, t_env *env_list);
int						execute_ast(t_ast *node, t_env *env_list);

/*memory management*/
void					free_env(t_env *env);
void					memory_management(t_env *env, int is_free_env);
t_gr_cl					*last_node(t_gr_cl *gar);
t_gr_cl					*new_node(void *ptr);
void					free_garbage_collector(t_gr_cl *garbage);
void					garbage_collector(void *new_pointer, int should_free);
void					*gc_malloc(size_t size);

/*tokenization*/
t_token					*new_token(char *str, t_type type, int *field);
t_token					*last_token(t_token *lst);
void					add_back(t_token **token, t_token *new);
int						operator_sep(char *input, t_info info, int *i,
							t_token **token);
int						word_sep(char *input, t_info info, int *i,
							t_token **token);
int						operator(char c);
void					is_qoute(char c, int *i, int *j);
void					init_struct(t_info **info);
t_token					*tokenization(char *input, int i, int check,
							t_token *token);

/*expansion function*/
char					*handle_default_dollar(void);
char					*handle_digit_var(int *index);
char					*handle_alpha_var(char *token, t_env *env, int *index);
char					*handle_exit_status(int *index);
char					*check_var(char *token, t_env *env, int *index);
int						handle_single_quote(int *db_quote, int *sg_quote,
							int *quote_state);
int						handle_double_quote(int *db_quote, int *sg_quote,
							int *quote_state);
int						which_type(char c, int *db_quote, int *sg_quote,
							int *quote_state);
t_token					*list_token(t_extoken *extoken, int i, int l, int word);
void					delete_node(t_token **arg_list, t_token **original_arg);
void					remove_extra_quote(char *token, int *field);
void					set_ambigouse(t_token *current);
void					insert_expanded_tokens(t_token *current,
							t_token **arg_list);
void					skip_var(char *token, int *i);
void					write_value(t_extoken *ex_token, char *value);
int						*ft_subint(int *field, int start, int len);
void					add_new_token(t_token **head, char *value, int *field,
							t_int db_int);
int						i_o(char c);
void					split_expanded_token(t_extoken *extoken,
							t_token **arg_list,
							t_token **original_arg,
							int herdoc);
void					fill_extoken_fields(t_extoken *ex_token, char *token,
							t_env *env, int k);
t_extoken				*expanded_token(char *token, t_env *env, int k);
void					process_token_expansion(t_token **current,
							t_token **next,
							t_token **arg_list,
							t_env *env);
void					expansion(t_token **arg_list, t_env *env);

void					setup_signals(void);
void					replace_variable(int *flag, t_env *env_list,
							t_token *token);
void					remove_quote(char *token, int sg_quote, int db_quote);
void					free_double_array(char **db_str);
void					child_process(int fd, char *delimiter);
int						parent_process(pid_t pid, int fd, int *ctrc);
char					*read_from_pipe(int pipefd);
char					*join(char *s1, char *s2);
t_ast					*init_command_ast(void);
int						is_word_token(t_type type);
int						redir_list_helper(t_ast **redir_head,
							t_ast **redir_tail);
void					sigint_handler_child(int signum);
char					*read_heredoc_lines(char *delimiter, int *ctrc);
char					*parse_herdoc_helper(int *i, int *ctrc);
int						parse_herdoc(t_ast **redir_head, t_ast **redir_tail,
							int *ctrc);
void					setup_signals(void);
int						check_for_var(char *key, t_env *env_list);
int						is_di_or_builtin(t_ast *node, t_env *env_list, int i);
int						is_absolute_path(t_ast *node, t_env *env_lsit);
int						is_dir(t_ast *node, t_env *env_list);
int						is_executable(t_ast *node, t_env *env_list);
int						is_path(t_ast *node, t_env *env_list);
char					*ft_strdup2(const char *s1);
char					*ft_substr2(char const *s, unsigned int start,
							size_t len);
void					memory_management(t_env *env, int free_env);
char					*ft_substr2(char const *s, unsigned int start,
							size_t len);
void					expand_evrything(t_ast *node, t_env *env_list);
t_variabls				*s_var(void);
int						execute_compound_command(t_ast *node, t_env *env_list);
char					**convert_env_to_array(t_env *env_list);
char					*find_cmd_path(char *cmd, t_env *env_list);
int						execute_pipe(t_ast *node, t_env *env_list,
							int input_fd);
int						handle_simple_command(t_ast *node, t_env *env_list,
							char **argv);
int						setup_redirections(t_ast *redir_list, t_env *env_list);
int						check_nm_var(char *str);
char					**token_list_to_argv(t_token *token_list);
int						check_for_var(char *key, t_env *env_list);
int						if_builtin(char *cmd);
int						which_one(int flag, t_token *token, t_env *env_list);
int						cmd_or_builtin(t_token *token, t_env *env_list,
							char **argv);
int						builtin_exit(t_token *token);
long long				ft_stoi(const char *str);
int						if_all_num(char *str);
int						builtin_export(t_token *token, t_env **env_list);
int						builtin_unset(t_token *token, t_env *env_list);
int						builtin_env(t_token *token, t_env *env_list);
int						builtin_pwd(t_token *token, t_env *env_list);
int						builtin_cd(t_token *token, t_env *env_list);
int						cd_helper(char *new_path, t_env *env_list,
							t_token *token);
int						builtin_echo(t_token *token, t_env *env_list);
int						execute_command(t_ast *node, t_env *env_list);
int						execute_ast(t_ast *node, t_env *env_list);
void					wildcard(t_token **arg_list);
void					update_env(char *key, char *new_value, t_env *env_list);
char					*get_env_value(char *key, t_env *env_list);
t_env					*init_env(char **envp);
void					env_add_back(t_env **lst, t_env *new);
t_env					*find_last(t_env *lst);
int						find_chr_pos(char *str, char c);
t_gr_cl					*last_node(t_gr_cl *gar);
t_gr_cl					*new_node(void *ptr);
void					free_garbage_collector(t_gr_cl *garbage);
void					garbage_collector(void *new_pointer, int should_free);
void					*gc_malloc(size_t size);
int						is_red_list(char *str);
t_token					*peek(void);
t_token					*consume(void);
void					*syntax_error(int status);
t_ast					*parse_simple_command(int *ctrc);
t_ast					*parse_subshell(int *ctrc);
t_ast					*parse_command(int *ctrc);
t_ast					*parse_pipeline(int *ctrc);
t_ast					*parse_compound_command(bool subshell, int *ctrc);
t_ast					*parse_redir_list(void);
void					append_token(t_token **head, t_token *new_token);
t_node_type				convert_type(t_type op);
int						execute_simple_cmd(t_env *env_list, char **argv);
t_token					*single_lst(t_token *token);
t_ast					*create_ast_node(t_ast *left, t_ast *right,
							t_token *token_list, t_node_type type);

#endif
