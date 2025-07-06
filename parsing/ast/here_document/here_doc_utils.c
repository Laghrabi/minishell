/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 21:15:30 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/05 21:18:59 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

char	*join(char *s1, char *s2)
{
	char	*new;

	int (i), (j);
	i = 0;
	j = 0;
	if (!s1)
	{
		new = gc_malloc(sizeof(char) * (ft_strlen(s2) + 1));
		if (!new)
			return (NULL);
		while (s2[j])
			new[i++] = s2[j++];
		return (new[i] = '\0', new);
	}
	new = gc_malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!new)
		return (NULL);
	while (s1[i])
		new[j++] = s1[i++];
	i = 0;
	while (s2[i])
		new[j++] = s2[i++];
	new[j] = '\0';
	return (new);
}

int is_quote(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == '\"' || str[i] == '\'')
            return (1);
        i++;
    }
    return (0);
}
//norm dyal ziad
void remove_quote(char *token)
{
    int (i), (j), (sg_quote), (db_quote);
    i = 0;
    j = 0;
    sg_quote = 0;
    db_quote = 0;
    while (token[i])
    {
        if (token[i] == '\'' && sg_quote == 0 && db_quote == 0)
            sg_quote = 1;
        else if (token[i] == '\'' && sg_quote == 1)
            sg_quote = 0;
        else if (token[i] == '\"' && sg_quote == 0 && db_quote == 0)
            db_quote = 1;
        else if (token[i] == '\"' && db_quote == 1)
            db_quote = 0;  
        if (((token[i] == '\'' && db_quote == 0) || (token[i] == '\"' && sg_quote == 0)))
        {
            j = i;
            while (token[j])
            {
                token[j] = token[j + 1];
                j++;
            }
            continue;
        }
        i++;
    }
}

char	*read_heredoc_lines(char *delimiter, int *ctrc)
{
    pid_t   (pid);
    int     (pipefd[2]);
    if (pipe(pipefd) == -1)
        return (NULL);
    pid = fork();
    if (pid < 0)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        return (NULL);
    }
    if (pid == 0)
		child_process(pipefd, delimiter);
    else
    {
        if (parent_process(pid, pipefd, ctrc) == 1)
			return (NULL);
    }
	return (read_from_pipe(pipefd[0]));
}

char	*parse_herdoc_helper(int *i, int *ctrc)
{
	char	*str;
	char	*buffer;

	str = peek()->value;
	if (is_quote(str))
		*i = 0;
	remove_quote(str);
	buffer = read_heredoc_lines(str, ctrc);
	return (buffer);
}
