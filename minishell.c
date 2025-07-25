/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:39:50 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/12 22:07:32 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*g_token;

void	handle_sigint(int signum)
{
	rl_replace_line("", 0);
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
	s_var()->exit_status = 128 + signum;
}

void	setup_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

int	minishell(int ctrc, char *input, t_env *env_list, t_ast *ast)
{
	while (1)
	{
		ctrc = 0;
		s_var()->printed = 0;
		input = readline("minishell$ ");
		if (input == NULL)
		{
			memory_management(env_list, 1);
			return (printf("exit\n"), s_var()->exit_status);
		}
		if (input[0] != '\0')
			add_history(input);
		g_token = tokenization(input, 0, 1, NULL);
		if (g_token == NULL)
			continue ;
		free(input);
		if (peek())
			ast = parse_compound_command(false, &ctrc);
		if (ast && s_var()->syntax_error != 2)
			s_var()->exit_status = execute_ast(ast, env_list);
		if (s_var()->syntax_error == 2)
			s_var()->syntax_error = 0;
		memory_management(NULL, 0);
	}
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	char *(input);
	t_ast *(ast);
	int (ctrc);
	t_env *(env_list);
	(void)ac;
	(void)av;
	ctrc = 0;
	input = NULL;
	ast = NULL;
	env_list = init_env(envp);
	s_var()->env_list = &env_list;
	s_var()->syntax_error = 0;
	s_var()->exit_status = 0;
	setup_signals();
	minishell(ctrc, input, env_list, ast);
	return (s_var()->exit_status);
}
