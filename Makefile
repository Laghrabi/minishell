CC = cc
CFLAGS = -Wall -Werror -Wextra -g  -fsanitize=address
LDFLAGS = -lreadline
SRC = minishell.c parsing/tokenization/tokenizer.c \
		parsing/tokenization/seperators_function.c \
		parsing/memory_leak/garbage_collector.c \
		parsing/memory_leak/memory_management.c \
		parsing/expansion/env_list.c \
		parsing/expansion/expansion.c \
		parsing/expansion/expansion_outils.c \
		wildcard/wildcard.c \
		builtins/builtins_utils.c ast_traversing/traversing.c \
		ast_traversing/utils1.c ast_traversing/utils2.c \
		ast_traversing/execute_pipe.c \
		ast_traversing/handle_error_messages.c \
		utils/static.c builtins/echo.c \
		builtins/cd.c builtins/pwd.c \
		builtins/unset.c builtins/export.c \
		builtins/env.c builtins/exit.c \
		parsing/ast/command.c parsing/ast/subshell.c \
		parsing/ast/redir_list.c parsing/ast/pipeline.c \
		parsing/ast/simple_command.c parsing/ast/compound_command.c \
		parsing/ast/here_document/here_doc.c \
		parsing/ast/here_document/here_doc_signals.c \
		parsing/ast/here_document/here_doc_utils.c
 
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
