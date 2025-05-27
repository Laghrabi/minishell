/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:39:50 by zfarouk           #+#    #+#             */
/*   Updated: 2025/05/27 17:50:25 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *g_token;

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
		node = gc_malloc(sizeof(t_env));
		if (node == NULL)
		{
			garbage_collector(NULL, 1);
			return (NULL);
		}
		pos = find_chr_pos(envp[i], '=');
		node->key = ft_substr(envp[i], 0, pos);
		node->value = ft_substr(envp[i], pos + 1, ft_strlen(envp[i]) - pos - 1);
		node->next = NULL;
		env_add_back(&head, node);
		i++;
	}
	return (head);
}

void print_indent(int depth)
{
    for (int i = 0; i < depth; ++i)
        printf("  ");  // two spaces per depth level
}

void print_node_type(t_node_type type)
{
    if (type == NODE_AND)
        printf("AND\n");
    else if (type == NODE_OR)
        printf("OR\n");
    else if (type == NODE_PIPE)
        printf("PIPE\n");
    else if (type == NODE_CMD)
        printf("CMD\n");
    else if (type == NODE_ARGS_LIST)
        printf("ARGS_LIST\n");
    else if (type == NODE_SUBSHELL)
        printf("SUBSHELL\n");
    else if (type == NODE_IREDIR)
        printf("REDIR_IN\n");
    else if (type == NODE_OREDIR)
        printf("REDIR_OUT\n");
    else if (type == NODE_APPEND)
        printf("REDIR_APPEND\n");
    else if (type == NODE_HEREDOC)
        printf("HEREDOC\n");
    else
        printf("UNKNOWN\n");
}

void print_token_list(t_token *tokens, int depth)
{
    t_token *current = tokens;
    while (current)
    {
        print_indent(depth);
        printf("TOKEN: [%s]\n", current->value);
        current = current->next;
    }
}

void print_ast(t_ast *node, int depth)
{
    if (!node)
        return;

    print_indent(depth);
    printf("Node Type: ");
    print_node_type(node->type);

    if (node->token_list)
    {
        print_indent(depth);
        printf("Token List:\n");
        print_token_list(node->token_list, depth + 1);
    }

    if (node->left)
    {
        print_indent(depth);
        printf("Left:\n");
        print_ast(node->left, depth + 1);
    }

    if (node->right)
    {
        print_indent(depth);
        printf("Right:\n");
        print_ast(node->right, depth + 1);
    }
}


int main(int ac, char **av, char **envp)
{
    char *input;
    t_ast *ast;
    //t_token *token;
    (void)ac;
    (void)av;
	t_env	*env_list = init_env(envp);
    while (1)
    {
        input = readline("minishell$ ");
        add_history(input);
        //printf("%s\n", input);
        g_token = tokenization(input);
        if (g_token == NULL)
            continue;
        free(input);
        t_token *current = g_token;
        while (current)
        {
            printf("[%s]  [%d]\n", current->value, current->token);
            current = current->next;
            printf("\n");
        }
        if (peek())
            ast = parse_compound_command(false);
        if (ast)
            print_ast(ast, 0);
        garbage_collector(NULL, 1);
    }
    return 0;
}