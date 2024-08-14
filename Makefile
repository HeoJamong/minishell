CC = cc
CFLAGS = -Wall -Wextra -Werror -g #-fsanitize=address

NAME =	minishell
SRC =	main.c \
		mini_signal.c

OBJ =	$(SRC:.c=.o)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	@make re -C ./Libft
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) \
	./Libft/libft.a \
	-lreadline
	
clean:
	@make clean -C ./Libft
	@rm -f $(OBJ)

fclean:	clean
	@make fclean -C ./Libft
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
