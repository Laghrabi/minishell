CC = cc
CFLAGS = -Wall -Werror -Wextra
LDFLAGS = -lreadline
SRC = minishell.c parsing/tokenization/tokenizer.c parsing/tokenization/seperators_function.c parsing/ast/ast.c parsing/ast/ast_utils.c parsing/memory_leak/garbage_collector.c

# SRC_BONUS = 
LIBFT = libft/libft.a
NAME = minishell
# NAME_BONUS = so_long_bonus
OBJECT = $(SRC:%.c=%.o)
# OBJECT_BONUS = $(SRC_BONUS:%.c=%.o )

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