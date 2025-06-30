/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:39:50 by zfarouk           #+#    #+#             */
/*   Updated: 2025/06/30 22:37:39 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *g_token;

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
        print_indent(depth);
        printf("is empty: [%d]   ", current->empty);
        printf("is heredoc: [%d]   ", current->is_herdoc);
        printf("is ambigeouse: [%d]   ", current->ambiguous);
        printf("is expansion: [%d]\n", current->expansion);
        print_indent(depth);
        printf("\n");
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


void print_env(t_env *env)
{
    t_env *current;

    current = env;
    while (current)
    {
        printf("variable : %s -> valeur : %s\n", current->key, current->value);
        current = current->next;
    }
}




void expand_evrything(t_ast *node, t_env *env_list)
{
    if (!node || !env_list)
        return;

    if (node->token_list)
    {
        expansion(&(node->token_list), env_list);
        wildcard(&(node->token_list));
    }
    if (node->left)
        expand_evrything(node->left, env_list);
    if (node->right)
        expand_evrything(node->right, env_list);
}
void memory_management(t_env *env, int free_env)
{
    t_env *current;

    garbage_collector(NULL, 1);
    if (free_env)
    {
        if (env)
        {
            while (env)
            {
                current = env->next;
                free(env->key);
                free(env->value);
                env = current;
            }
        }
    }
}

void	handle_sigint(int signum)
{
	(void)signum;
	rl_replace_line("", 0);
    write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}

void	setup_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

int main(int ac, char **av, char **envp)
{
    char *input;
    t_ast *ast;
    int     c;

    (void)ac;
    (void)av;
	t_env	*env_list = init_env(envp);
    setup_signals();
    while (1)
    {
        input = readline("minishell$ ");
        if (input == NULL)
            return(printf("exit\n"), 0);
        add_history(input);
        g_token = tokenization(input);
        if (g_token == NULL)
            continue;
        free(input);
        if (peek())
            ast = parse_compound_command(false);
        // test_expansion(ast, env_list);
        // if (ast)
        //     print_ast(ast, 0);
        c = execute_ast(ast, env_list);
        printf ("STATUS=%d\n", c);
        memory_management( NULL, 0);
    }
    return (s_var()->exit_status);
}
