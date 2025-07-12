/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 03:44:43 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/12 22:38:49 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	rewrite(char *value)
{
	int		fd;
	char	*name;

	name = creat_herdoc_file();
	unlink_func(name, 0);
	fd = open(name, O_RDWR);
	write(fd, value, ft_strlen(value));
	close(fd);
	fd = open(name, O_RDWR);
	return (fd);
}

int	open_herdoc_file(char *text, int expans)
{
	int		fd;
	char	*next_line;
	char	*expan;
	t_token	*yes;

	expan = NULL;
	fd = open(text, O_RDONLY);
	next_line = get_next_line(fd);
	while (next_line)
	{
		expan = ft_strjoin(expan, next_line);
		free(next_line);
		next_line = get_next_line(fd);
	}
	yes = new_token(expan, T_WORD, NULL);
	yes->is_herdoc = 7;
	yes->expansion = expans;
	expansion(&yes, *(s_var()->env_list));
	close(fd);
	fd = rewrite(yes->value);
	return (fd);
}

int	heredoc_to_fd(t_ast *heredoc_node, t_env *env_list)
{
	(void)env_list;
	if (heredoc_node == NULL || heredoc_node->token_list == NULL)
		return (-1);
	return (open_herdoc_file(heredoc_node->token_list->value,
			heredoc_node->token_list->expansion));
}

int	handle_one_redirection(t_ast *redir, t_env *env_list)
{
	int		fd;
	char	*filename;

	if (redir->type != NODE_HEREDOC)
	{
		filename = get_file_name(redir);
		if (filename == NULL)
			return (-1);
		if (redir->type == NODE_OREDIR)
			fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir->type == NODE_IREDIR)
			fd = open(filename, O_RDONLY);
		else if (redir->type == NODE_APPEND)
			fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			return (-1);
	}
	else
		fd = heredoc_to_fd(redir, env_list);
	if (fd == -1)
		perror("minishell");
	return (fd);
}

int	setup_redirections(t_ast *redir_list, t_env *env_list)
{
	int	fd;

	if (redir_list == NULL || env_list == NULL)
		return (1);
	while (redir_list != NULL)
	{
		fd = handle_one_redirection(redir_list, env_list);
		if (fd == -1)
			return (1);
		if (redir_list->type == NODE_OREDIR || redir_list->type == NODE_APPEND)
			dup2(fd, STDOUT_FILENO);
		else
			dup2(fd, STDIN_FILENO);
		close(fd);
		redir_list = redir_list->right;
	}
	return (0);
}
