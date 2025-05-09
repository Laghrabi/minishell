CC = cc
CFLAGS = -Wall -Werror -Wextra
LDFLAGS = -lreadline
SRC = minishell.c parcing/tokenizer.c parcing/seperators_function.c

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
	$(CC) $(OBJECT) $(LIBFT) -o $(NAME)  $(LDFLAGS)



%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


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