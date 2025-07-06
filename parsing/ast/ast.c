// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   ast.c                                              :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/05/13 16:31:39 by claghrab          #+#    #+#             */
// /*   Updated: 2025/07/06 15:54:48 by zfarouk          ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "../../minishell.h"

// t_ast   *parse_compound_command(bool subshell, int *ctrc)
// {
//     t_type      (op_type);
//     t_node_type (node_type);
//     t_ast (*left), (*right), (*node);
//     left = parse_pipeline(ctrc);
//     if (left == NULL)
//         return (NULL);
//     while (peek() && (peek()->token == T_AND || peek()->token == T_OR))
//     {
//         op_type = consume()->token;
//         if (op_type == T_AND)
//             node_type = NODE_AND;
//         else
//             node_type = NODE_OR;
//         right = parse_compound_command(subshell, ctrc);
//         if (right == NULL)
//             return (NULL);
//         node = create_ast_node(left, right, NULL, node_type);
//         if (node == NULL)
//             return (NULL);
//         left = node;
//     }
//     if (peek() && ((peek()->token == T_RPAREN && subshell == false) || (peek()->token != T_AND && peek()->token != T_OR && subshell == false)))
//         return (syntax_error(2));
//     return (left);
// }

// t_ast   *parse_pipeline(int *ctrc)
// {
//     t_ast   *left;
//     t_ast   *right;
//     t_ast   *node;

//     left = parse_command(ctrc);
//     if (left == NULL)
//         return (NULL);
//     while (peek() && peek()->token == T_PIPE)
//     {
//         consume();
//         right = parse_pipeline(ctrc);
//         if (right == NULL)
//             return (NULL);
//         node = create_ast_node(left, right, NULL, NODE_PIPE);
//         if (node == NULL)
//             return (NULL);
//         left = node;
//     }
//     return (left);
// }

// t_ast   *parse_command(int *ctrc)
// {
//     t_ast   *node;
//     if (peek() && peek()->token == T_LPAREN)
//         return (parse_subshell(ctrc));
//     else if (peek() && (peek()->token == T_WORD || peek()->token == T_DOLLAR_S || peek()->token == T_SINGLE_Q || peek()->token == T_DOUBLE_Q || peek()->token == T_WILDCARD || is_red_list(peek()->value) == 1))
//     {
//         node = parse_simple_command(ctrc);
//         return (node);
//     }
//     else
//         return (syntax_error(2));
// }

// t_ast   *parse_subshell(int *ctrc)
// {
//     t_ast   *inner_command;
//     t_ast   *red_list;
//     t_ast   *node;

//     red_list = NULL;
//     if (peek() == NULL || peek()->token != T_LPAREN)
//         return (syntax_error(2));
//     consume();
//     inner_command = parse_compound_command(true, ctrc);
//     if (peek() == NULL || peek()->token != T_RPAREN)
//         return (syntax_error(2));
//     consume();
//     if (peek() && is_red_list(peek()->value) == 1)
//         red_list = parse_redir_list();
//     node = create_ast_node(inner_command, red_list, NULL, NODE_SUBSHELL);
//     if (node == NULL)
//         return (NULL);
//     return (node);
// }

// int	redir_list_helper(t_ast **redir_head, t_ast **redir_tail)
// {
// 	t_type  (op_type);
//     t_token (*file_name);
// 	t_ast   (*redir);
// 	op_type = consume()->token;
//     if (!peek() || (peek()->token != T_WORD && peek()->token != T_DOLLAR_S && peek()->token != T_SINGLE_Q && peek()->token != T_DOUBLE_Q))
//         return (1);
//     file_name = consume();
//     redir = create_ast_node(NULL, NULL, single_token_list(file_name), convert_t_type(op_type));
//     if (*redir_head == NULL)
//     {
//     	*redir_head = redir;
//         *redir_tail = redir;
//     }
//     else
//     {
//     	(*redir_tail)->right = redir;
//     	*redir_tail = redir;
//     }
// 	return (0);
// }

// char	*join(char *s1, char *s2)
// {
// 	char	*new;

// 	int (i), (j);
// 	i = 0;
// 	j = 0;
// 	if (!s1)
// 	{
// 		new = gc_malloc(sizeof(char) * (ft_strlen(s2) + 1));
// 		if (!new)
// 			return (NULL);
// 		while (s2[j])
// 			new[i++] = s2[j++];
// 		return (new[i] = '\0', new);
// 	}
// 	new = gc_malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
// 	if (!new)
// 		return (NULL);
// 	while (s1[i])
// 		new[j++] = s1[i++];
// 	i = 0;
// 	while (s2[i])
// 		new[j++] = s2[i++];
// 	new[j] = '\0';
// 	return (new);
// }

// int is_quote(char *str)
// {
//     int i;

//     i = 0;
//     while (str[i])
//     {
//         if (str[i] == '\"' || str[i] == '\'')
//             return (1);
//         i++;
//     }
//     return (0);
// }
// //norm dyal ziad
// void remove_quote(char *token)
// {
//     int (i), (j), (sg_quote), (db_quote);
//     i = 0;
//     j = 0;
//     sg_quote = 0;
//     db_quote = 0;
//     while (token[i])
//     {
//         if (token[i] == '\'' && sg_quote == 0 && db_quote == 0)
//             sg_quote = 1;
//         else if (token[i] == '\'' && sg_quote == 1)
//             sg_quote = 0;
//         else if (token[i] == '\"' && sg_quote == 0 && db_quote == 0)
//             db_quote = 1;
//         else if (token[i] == '\"' && db_quote == 1)
//             db_quote = 0;  
//         if (((token[i] == '\'' && db_quote == 0) || (token[i] == '\"' && sg_quote == 0)))
//         {
//             j = i;
//             while (token[j])
//             {
//                 token[j] = token[j + 1];
//                 j++;
//             }
//             continue;
//         }
//         i++;
//     }
// }

// static void sigint_handler_child(int signum)
// {
//     (void)signum;
//     write(STDOUT_FILENO, "\n", 1);
//     exit(130);
// }

// void	child_process(int pipefd[2], char *delimiter)
// {
// 	char	*line;
	
// 	signal(SIGINT, sigint_handler_child);
// 	close(pipefd[0]);
// 	while(1)
// 	{
// 		line = readline("> ");
// 		if (line == NULL)
// 		{
// 			printf("here-doc delemited by EOF\n");
// 			break ;
// 		}
// 		if (line && ft_strcmp(line, delimiter) == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		write(pipefd[1], line, ft_strlen(line));
// 		write(pipefd[1], "\n", 1);
// 		free(line);
// 	}
// 	close(pipefd[1]);
// 	exit (0);
// }

// int	parent_process(pid_t pid, int pipefd[2], int *ctrc)
// {
// 	int	status;
	
// 	close(pipefd[1]);
// 	signal(SIGINT, SIG_IGN);
// 	waitpid(pid, &status, 0);
// 	setup_signals();
// 	if (WEXITSTATUS(status))
// 	{
// 		s_var()->exit_status = 130;
// 		*ctrc = 1;
// 		close(pipefd[0]);
// 		return (1);
// 	}
// 	return (0);
// }

// char	*read_from_pipe(int pipefd)
// {
// 	char (read_buf[1024]), (*buffer);
// 	ssize_t (n);
// 	buffer = NULL;
//     n = read(pipefd, read_buf, sizeof(read_buf) - 1);
//     while (n > 0)
//     {
//         read_buf[n] = '\0';
//         buffer = join(buffer, read_buf);
//         n = read(pipefd, read_buf, sizeof(read_buf) - 1);
//     }
//     close(pipefd);
//     return (buffer);
// }

// char	*read_heredoc_lines(char *delimiter, int *ctrc)
// {
//     pid_t   (pid);
//     int     (pipefd[2]);
//     if (pipe(pipefd) == -1)
//         return (NULL);
//     pid = fork();
//     if (pid < 0)
//     {
//         close(pipefd[0]);
//         close(pipefd[1]);
//         return (NULL);
//     }
//     if (pid == 0)
// 		child_process(pipefd, delimiter);
//     else
//     {
//         if (parent_process(pid, pipefd, ctrc) == 1)
// 			return (NULL);
//     }
// 	return (read_from_pipe(pipefd[0]));
// }

// char	*parse_herdoc_helper(int *i, int *ctrc)
// {
// 	char	*str;
// 	char	*buffer;

// 	str = peek()->value;
// 	if (is_quote(str))
// 		*i = 0;
// 	remove_quote(str);
// 	buffer = read_heredoc_lines(str, ctrc);
// 	return (buffer);
// }

// int	parse_herdoc(t_ast **redir_head, t_ast **redir_tail, int *ctrc)
// {
// 	t_type  (op_type);
//     char	(*text);
// 	t_ast   (*redir);
// 	op_type = consume()->token;
//     if (!peek() || (peek()->token != T_WORD && peek()->token != T_DOLLAR_S && peek()->token != T_SINGLE_Q && peek()->token != T_DOUBLE_Q))
//     	return (1);
//     text = parse_herdoc_helper(&(peek()->expansion), ctrc);
//     peek()->value = text;
//     peek()->is_herdoc = 7;
//     redir = create_ast_node(NULL, NULL, single_token_list(consume()), convert_t_type(op_type));
//     if (*redir_head == NULL)
//     {
//     	*redir_head = redir;
//         *redir_tail = redir;
//     }
//     else
//     {
//     	(*redir_tail)->right = redir;
//     	*redir_tail = redir;
//     }
// 	return (0);
// }

// t_ast   *parse_simple_command(int *ctrc)
// {
//     t_ast   (*cmd), (*args_node),(*redir_head), (*redir_tail);
//     args_node = create_ast_node(NULL, NULL, NULL, NODE_ARGS_LIST);
//     redir_head = NULL;
// 	redir_tail = NULL;
// 	while (peek())
// 	{
//     	if (peek() && (peek()->token == T_WORD || peek()->token == T_DOLLAR_S || peek()->token == T_SINGLE_Q || peek()->token == T_DOUBLE_Q || peek()->token == T_WILDCARD))
//         	append_token(&args_node->token_list, consume());
//     	else if (peek() && is_red_list(peek()->value) == 1)
//         {
// 			if (peek()->token == T_HEREDOC)
//             {
//                 if (*ctrc == 0)
//                 {
// 				    if (parse_herdoc(&redir_head, &redir_tail, ctrc) == 1)
//                         return (syntax_error(2));
//                 }
//                 else
//                 {
//                     consume();
//                     consume();    
//                 }
//             }
// 			else if (redir_list_helper(&redir_head, &redir_tail) == 1)
// 				return (syntax_error(2));
// 		}
// 		else
// 			break ;
// 	}
//     if (!args_node->token_list && !redir_head && *ctrc == 0)
//         return (syntax_error(2));
//     cmd = create_ast_node(args_node, redir_head, NULL, NODE_CMD);
//     return (cmd);
// }

// static t_ast *init_command_ast(void)
// {
//     return create_ast_node(NULL, NULL, NULL, NODE_ARGS_LIST);
// }

// static int is_word_token(t_type type)
// {
//     return (type == T_WORD || type == T_DOLLAR_S || 
//             type == T_SINGLE_Q || type == T_DOUBLE_Q || 
//             type == T_WILDCARD);
// }

// static int handle_word_token(t_ast *args_node)
// {
//     if (peek() && is_word_token(peek()->token))
// 	{
//         append_token(&args_node->token_list, consume());
//         return (0);
//     }
//     return (1);
// }

// static int handle_heredoc(t_ast **redir_head, t_ast **redir_tail, int *ctrc)
// {
//     if (*ctrc == 0)
// 	{
//         if (parse_herdoc(redir_head, redir_tail, ctrc) == 1)
//             return (1);
//     }
// 	else
// 	{
//         consume();
//         consume();
//     }
//     return (0);
// }

// static int handle_redirection(t_ast **redir_head, t_ast **redir_tail, int *ctrc)
// {
//     if (peek()->token == T_HEREDOC)
// 	{
//         if (handle_heredoc(redir_head, redir_tail, ctrc) == 1)
//             return (1);
//     }
// 	else if (redir_list_helper(redir_head, redir_tail) == 1)
//         return (1);
//     return (0);
// }

// static int validate_command(t_ast *args_node, t_ast *redir_head, int ctrc)
// {
//     if (!args_node->token_list && !redir_head && ctrc == 0)
//         return (1);
//     return (0);
// }

// t_ast *parse_simple_command(int *ctrc)
// {
//     t_ast (*args_node), (*redir_head), (*redir_tail);
// 	args_node = init_command_ast();
// 	redir_head = NULL;
// 	redir_tail = NULL;
//     while (peek())
// 	{
//         if (handle_word_token(args_node) == 0)
//             continue ;
//         if (peek() && is_red_list(peek()->value) == 1) {
//             if (handle_redirection(&redir_head, &redir_tail, ctrc) == 1)
//                 return (syntax_error(2));
//             continue ;
//         }
//         break ;
//     }
//     if (validate_command(args_node, redir_head, *ctrc) == 1)
//         return (syntax_error(2));
//     return (create_ast_node(args_node, redir_head, NULL, NODE_CMD));
// }

// t_ast   *parse_redir_list(void)
// {
//     t_type  (op_type);
//     t_token (*file_name);
//     t_ast   (*redir),(*redir_head), (*redir_tail);
//     redir_head = NULL;
//     redir_tail = NULL;
//     while (peek() && is_red_list(peek()->value) == 1)
//     {
//         op_type = consume()->token;
//         if (!peek() || (peek()->token != T_WORD && peek()->token != T_DOLLAR_S && peek()->token != T_SINGLE_Q && peek()->token != T_DOUBLE_Q))
//             return (syntax_error(2));
//         file_name = consume();
//         redir = create_ast_node(NULL, NULL, single_token_list(file_name), convert_t_type(op_type));
//         if (redir_head == NULL)
//         {
//             redir_head = redir;
//             redir_tail = redir;
//         }
//         else
//         {
//             redir_tail->right = redir;
//             redir_tail = redir;
//         }
//     }
//     return (redir_head);
// }

// t_ast   *parse_simple_command(void)
// {
//     t_ast   (*cmd), (*args_node),(*redir_head), (*new_redir);
//     args_node = create_ast_node(NULL, NULL, NULL, NODE_ARGS_LIST);
//     redir_head = NULL;
// 	while(peek())
// 	{
//     	if (peek() && (peek()->token == T_WORD || peek()->token == T_DOLLAR_S || peek()->token == T_SINGLE_Q || peek()->token == T_DOUBLE_Q || peek()->token == T_WILDCARD))
//         	append_token(&args_node->token_list, consume());
//     	else if (peek() && is_red_list(peek()->value) == 1)
// 		{
//         	new_redir = parse_redir_list();
// 			if (new_redir == NULL)
// 				return (syntax_error(2));
// 			if (redir_head == NULL)
// 				redir_head = new_redir;
// 			else
// 			{
				
// 			}
// 		}
// 	}
//     if (!args_node->token_list && !redir_head)
//         return (syntax_error(2));
//     cmd = create_ast_node(args_node, redir_head, NULL, NODE_CMD);
//     return (cmd);
// }
