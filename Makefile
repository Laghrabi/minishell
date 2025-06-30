CC = cc
CFLAGS = -Wall -Werror -Wextra -g # -fsanitize=address
LDFLAGS = -lreadline
SRC = minishell.c parsing/tokenization/tokenizer.c \
		parsing/tokenization/seperators_function.c \
		parsing/ast/ast.c parsing/ast/ast_utils.c \
		parsing/memory_leak/garbage_collector.c \
		parsing/expansion/env_list.c \
		parsing/expansion/expansion.c \
		parsing/expansion/expansion_outils.c \
		wildcard/wildcard.c builtins/builtins.c \
		builtins/builtins_utils.c ast_traversing/traversing.c \
		ast_traversing/utils1.c ast_traversing/utils2.c \
		ast_traversing/execute_pipe.c \
		utils/static.c
 
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
