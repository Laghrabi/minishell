/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 21:13:04 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/05 21:19:33 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	child_process(int pipefd[2], char *delimiter)
{
	char	*line;
	
	signal(SIGINT, sigint_handler_child);
	close(pipefd[0]);
	while(1)
	{
		line = readline("> ");
		if (line == NULL)
		{
			printf("here-doc delemited by EOF\n");
			break ;
		}
		if (line && ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);
	exit (0);
}

int	parent_process(pid_t pid, int pipefd[2], int *ctrc)
{
	int	status;
	
	close(pipefd[1]);
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	setup_signals();
	if (WEXITSTATUS(status))
	{
		s_var()->exit_status = 130;
		*ctrc = 1;
		close(pipefd[0]);
		return (1);
	}
	return (0);
}

char	*read_from_pipe(int pipefd)
{
	char (read_buf[1024]), (*buffer);
	ssize_t (n);
	buffer = NULL;
    n = read(pipefd, read_buf, sizeof(read_buf) - 1);
    while (n > 0)
    {
        read_buf[n] = '\0';
        buffer = join(buffer, read_buf);
        n = read(pipefd, read_buf, sizeof(read_buf) - 1);
    }
    close(pipefd);
    return (buffer);
}

int	parse_herdoc(t_ast **redir_head, t_ast **redir_tail, int *ctrc)
{
	t_type  (op_type);
    char	(*text);
	t_ast   (*redir);
	op_type = consume()->token;
    if (!peek() || (peek()->token != T_WORD && peek()->token != T_DOLLAR_S && peek()->token != T_SINGLE_Q && peek()->token != T_DOUBLE_Q))
    	return (1);
    text = parse_herdoc_helper(&(peek()->expansion), ctrc);
    peek()->value = text;
    peek()->is_herdoc = 7;
    redir = create_ast_node(NULL, NULL, single_token_list(consume()), convert_t_type(op_type));
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