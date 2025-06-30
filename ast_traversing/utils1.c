/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 03:44:43 by claghrab          #+#    #+#             */
/*   Updated: 2025/06/30 20:43:28 by claghrab         ###   ########.fr       */
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
			printf("bash: ambiguous redirect\n");
			return (NULL);
		}
		return (redir_node->token_list->value);
	}
	return (NULL);
}

char **token_list_to_argv(t_token *token_list)
{
    char **argv;
    t_token *current;
    int i;
    
    if (token_list == NULL)
        return (NULL);
    i = 0;
    current = token_list;
    while (current != NULL)
    {
        i++;
        current = current->next;
    }
    argv = gc_malloc(sizeof(char *) * (i + 1));
    i = 0;
    current = token_list;
    while (current != NULL)
    {
        argv[i++] = ft_strdup(current->value);
        current = current->next;
    }
    argv[i] = NULL;
    return (argv);
}

int	heredoc_to_fd(t_ast *heredoc_node)
{
	int		pipefd[2];
	char	*input;

	if (heredoc_node == NULL || heredoc_node->token_list == NULL)
		return (-1);
	if (pipe(pipefd) == -1)
		return (-1);
	input = heredoc_node->token_list->value;
	write(pipefd[1], input, ft_strlen(input));
	close(pipefd[1]);
	return (pipefd[0]);	
}

int	setup_redirections(t_ast *redir_list, t_env *env_list)
{
	int		(fd);
	char	(*filename);
	if (redir_list == NULL || env_list == NULL)
		return (1);
	while (redir_list != NULL)
	{
		//printf("here=%d\n", redir_list->token_list->ambiguous);
		filename = get_file_name(redir_list);
		if (filename == NULL)
			return (1);
		if (redir_list->type == NODE_OREDIR)
			fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir_list->type == NODE_IREDIR)
			fd = open(filename, O_RDONLY);
		else if (redir_list->type == NODE_APPEND)
			fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (redir_list->type == NODE_HEREDOC)
			fd = heredoc_to_fd(redir_list);
		else
			return (1);
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
