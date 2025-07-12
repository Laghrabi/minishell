/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 03:44:43 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/12 01:20:56 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_file_name(t_ast *redir_node)
{
	if (redir_node == NULL)
		return (NULL);
	if (redir_node->token_list != NULL)
	{
		if (redir_node->token_list->ambiguous == 1)
		{
			ft_putstr_fd("minishell: ambiguous redirect\n", 2);
			return (NULL);
		}
		return (redir_node->token_list->value);
	}
	return (NULL);
}

int	open_herdoc_file(char *text)
{
	// char	*name;
	// char	*random;
	int		fd_random;

	// random = gc_malloc(sizeof(char) * 10);
	// ft_bzero(random, 10);
	// fd_random = open("/dev/random", O_RDONLY);
	// if (fd_random != -1)
	// {
	// 	read(fd_random, random, 9);
	// 	random[9] = '\0';
	// 	while (ft_strchr(random, '$') != NULL)
	// 	{
	// 		read(fd_random, random, 9);
	// 		random[9] = '\0';
	// 	}
	// }
	//close(fd_random);
	//name = ft_strjoin("/tmp/herdoc", random);
	//fd_random = open(name, O_CREAT | O_RDWR, 0644);
	//write(fd_random, text, ft_strlen(text));
	//close(fd_random);
	printf("TEXT:[%s]\n", text);
	fd_random = open(text, O_RDONLY);
	return (fd_random);
}

int	heredoc_to_fd(t_ast *heredoc_node, t_env *env_list)
{
	(void)env_list;
	if (heredoc_node == NULL || heredoc_node->token_list == NULL)
		return (-1);
	return (open_herdoc_file(heredoc_node->token_list->value));
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
	t_ast *tmp = redir_list;
	int i = 0;
	while (tmp) {
		printf("REDIR %d: type = %d, value = %s\n", i++, tmp->type, tmp->token_list->value);
		tmp = tmp->right;
	}
	while (redir_list != NULL)
	{
		printf("AGAIN\n");
		printf("TOKEN_LIST: [%s]\n", redir_list->token_list->value);
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
