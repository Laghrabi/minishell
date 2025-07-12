/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 21:13:04 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/12 17:41:22 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

char	*creat_herdoc_file(void)
{
	char	*name;
	char	*random;
	int		fd_random;

	random = gc_malloc(sizeof(char) * 10);
	ft_bzero(random, 10);
	fd_random = open("/dev/random", O_RDONLY);
	if (fd_random != -1)
	{
		read(fd_random, random, 9);
		random[9] = '\0';
		while (ft_strchr(random, '$') != NULL)
		{
			read(fd_random, random, 9);
			random[9] = '\0';
		}
	}
	close(fd_random);
	name = ft_strjoin("/tmp/herdoc", random);
	fd_random = open(name, O_CREAT | O_RDWR, 0644);
	close(fd_random);
	return (name);
}

void	child_process(int fd, char *delimiter)
{
	char	*line;

	signal(SIGINT, sigint_handler_child);
		while (1)
	{
		line = readline("> ");
		if (line == NULL)
		{
			ft_putstr_fd("here-doc delemited by EOF\n", 2);
			break ;
		}
		if (line && ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	memory_management(*(s_var()->env_list), 1);
	exit(0);
}

int	parent_process(pid_t pid, int fd, int *ctrc)
{
	int	status;

	close(fd);
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	setup_signals();
	if (WEXITSTATUS(status))
	{
		s_var()->exit_status = 130;
		s_var()->syntax_error = 2;
		*ctrc = 1;
		close(fd);
		return (1);
	}
	return (0);
}

char	*read_from_pipe(int fd)
{
	char	read_buf[1024];
	char	*buffer;
	ssize_t	n;

	buffer = NULL;
	n = read(fd, read_buf, sizeof(read_buf) - 1);
	while (n > 0)
	{
		read_buf[n] = '\0';
		buffer = join(buffer, read_buf);
		n = read(fd, read_buf, sizeof(read_buf) - 1);
	}
	close(fd);
	return (buffer);
}

int	parse_herdoc(t_ast **redir_head, t_ast **redir_tail, int *ctrc)
{
	t_type (op_type);
	char *(text);
	t_ast *(redir);
	op_type = consume()->token;
	if (!peek() || peek()->token != T_WORD)
		return (1);
	text = parse_herdoc_helper(&(peek()->expansion), ctrc);
	peek()->value = text;
	peek()->is_herdoc = 7;
	redir = create_ast_node(NULL, NULL, single_lst(consume()),
			convert_type(op_type));
	if (*redir_head == NULL)
	{
		*redir_head = redir;
		*redir_tail = redir;
	}
	else
	{
		(*redir_tail)->right = redir;
		*redir_tail = redir;
	}
	return (0);
}
