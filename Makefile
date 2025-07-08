CC = cc
CFLAGS = -Wall -Werror -Wextra -g  #-fsanitize=address
LDFLAGS = -lreadline
SRC = minishell.c parsing/tokenization/tokenizer.c \
		parsing/tokenization/seperators_function.c \
		parsing/ast/command.c parsing/ast/subshell.c \
		parsing/ast/redir_list.c parsing/ast/pipeline.c \
		parsing/ast/simple_command.c parsing/ast/compound_command.c \
		parsing/ast/here_document/here_doc.c \
		parsing/ast/here_document/here_doc_signals.c \
		parsing/ast/here_document/here_doc_utils.c \
		memory_management/garbage_collector.c \
		memory_management/memory_management.c \
		expansion/expansion.c \
		expansion/expansion_outils.c \
		expansion/expansion_outils1.c \
		expansion/expansion_check_var.c \
		expansion/expansion_handle_quote.c \
		env_list/env_list.c \
		wildcard/wildcard.c \
		builtins/builtins_utils.c ast_traversing/traversing.c \
		ast_traversing/utils1.c ast_traversing/utils2.c \
		ast_traversing/execute_pipe.c \
		ast_traversing/execute_compound_command.c \
		ast_traversing/execute_subshell_simple_command.c \
		ast_traversing/handle_error_messages.c \
		utils/static.c builtins/echo.c \
		builtins/cd.c builtins/pwd.c \
		builtins/unset.c builtins/export.c \
		builtins/env.c builtins/exit.c 
 
LIBFT = libft/libft.a
NAME = minishell
OBJECT = $(SRC:%.c=%.o)

all:  $(NAME)

$(LIBFT) :
	make -C libft

$(NAME): $(OBJECT) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJECT) $(LIBFT) -o $(NAME)  $(LDFLAGS)


# bonus: $(NAME_BONUS)

# $(NAME_BONUS) : $(OBJECT_BONUS) 
# 	$(CC) $(CFLAGS) $(SRC_BONUS) -o $(NAME_BONUS)

clean:
	rm -f $(OBJECT)
	$(MAKE) clean -C libft

fclean: clean
	rm -f $(NAME)
	$(MAKE) fclean -C libft

re: fclean all

.PHONY: clean fclean all re bonus
