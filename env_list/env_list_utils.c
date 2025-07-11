/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 14:41:04 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/11 17:12:14 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	find_chr_pos(char *str, char c)
{
	int	i;

	if (str == NULL)
		return (-1);
	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

char	*ft_substr2(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	slen;
	char	*p;

	if (!s)
		return (NULL);
	slen = ft_strlen(s);
	if (len > slen - start)
		len = slen - start;
	p = malloc((len + 1) * sizeof(char));
	if (!p)
		return (NULL);
	i = 0;
	while (i < len)
	{
		p[i] = s[start + i];
		i++;
	}
	p[i] = '\0';
	return (p);
}

void	copy_env_key_value(char **argv, int *i, t_env *env)
{
	char	*tmp;

	argv[*i] = ft_strdup2(env->key);
	if (env->value != NULL)
	{
		tmp = argv[*i];
		argv[*i] = ft_strjoin2(argv[*i], "=");
		free(tmp);
		tmp = argv[*i];
		argv[*i] = ft_strjoin2(argv[*i], env->value);
		free(tmp);
	}
	(*i)++;
}

char	**convert_env_to_array(t_env *env_list)
{
	char	**argv;
	t_env	*current;

	int (i), (size);
	if (env_list == NULL)
		return (NULL);
	size = 0;
	current = env_list;
	while (current != NULL)
	{
		size++;
		current = current->next;
	}
	argv = malloc(sizeof(char *) * (size + 1));
	i = 0;
	current = env_list;
	while (current != NULL)
	{
		if (ft_strcmp(current->key, "_") != 0)
			copy_env_key_value(argv, &i, current);
		current = current->next;
	}
	argv[i] = NULL;
	return (argv);
}
