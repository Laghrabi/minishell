CC = cc
CFLAGS = -Wall -Werror -Wextra
LDFLAGS = -lreadline
SRC = minishell.c parcing/tokenizer.c

# SRC_BONUS = 

NAME = minishell
# NAME_BONUS = so_long_bonus
OBJECT = $(SRC:%.c=%.o)
# OBJECT_BONUS = $(SRC_BONUS:%.c=%.o )

all:  $(NAME)

$(NAME): $(OBJECT)
	$(CC) $(OBJECT) -o $(NAME) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


# bonus: $(NAME_BONUS)

# $(NAME_BONUS) : $(OBJECT_BONUS) 
# 	$(CC) $(CFLAGS) $(SRC_BONUS) -o $(NAME_BONUS)

clean:
	rm -f $(OBJECT) 
fclean: clean
	rm -f $(NAME) 

re: fclean all

.PHONY: clean fclean all re bonus