/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:10:56 by claghrab          #+#    #+#             */
/*   Updated: 2025/07/08 20:47:59 by zfarouk          ###   ########.fr       */
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

t_env	*find_last(t_env *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	env_add_back(t_env **lst, t_env *new)
{
	t_env	*current;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	current = find_last(*lst);
	current->next = new;
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

void	increment_shelvl_value(t_env *envp)
{
	char	*string;
	int		integer;
	
	if (envp == NULL)
		return ;
	string = get_env_value("SHLVL", envp);
	integer = ft_atoi(string);
	integer++;
	string = ft_itoa(integer);
	update_env("SHLVL", string, envp);
}

t_env	*init_env(char **envp)
{
	int		i;
	int		pos;
	t_env	*head;
	t_env	*node;
	
	if (envp == NULL || *envp == NULL)
		return (NULL);
	i = 0;
	head = NULL;
	while (envp[i] != NULL)
	{
		node = malloc(sizeof(t_env));
		pos = find_chr_pos(envp[i], '=');
		node->key = ft_substr2(envp[i], 0, pos);
		node->value = ft_substr2(envp[i], pos + 1, ft_strlen(envp[i]) - pos - 1);
		node->next = NULL;
		env_add_back(&head, node);
		i++;
	}
	increment_shelvl_value(head);
	return (head);
}
