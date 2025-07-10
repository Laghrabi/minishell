/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claghrab <claghrab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:39:50 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/10 16:04:52 by claghrab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell.h"

t_token	*g_token;

void	handle_sigint(int signum)
{
	// if (s_var()->g_child_running == 0)
	// {
	//     rl_replace_line("", 0);
	//     write(STDOUT_FILENO, "\n", 1);
	//     rl_on_new_line();
	//     rl_redisplay();
	// }
	// else
	//     write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	//wach khasha tkteb f stdout wla stderr had ster li lta7t????
	//!!!!!!!
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

int	main(int ac, char **av, char **envp)
{
	char	*input;
	t_ast	*ast;
	int		ctrc;
	t_env	*env_list;

	(void)ac;
	(void)av;
	env_list = init_env(envp);
	setup_signals();
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
		// if (ast)
		// 	print_ast(ast, 0);
		if (ast && s_var()->syntax_error != 2)
		if (ast)
			print_ast(ast, 0);
		if (ast && s_var()->syntax_error != 2)
			s_var()->exit_status = execute_ast(ast, env_list);
		if (s_var()->syntax_error == 1)
			s_var()->syntax_error = 0;
		memory_management(NULL, 0);
	}
	return (s_var()->exit_status);
}
