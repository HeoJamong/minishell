CC = cc
CFLAGS = -Wall -Wextra -Werror -g #-fsanitize=address

NAME =	minishell
SRC =	main.c \
		ms_lst.c \
		ms_utils.c \
		ms_signal1.c \
		ms_signal2.c \
		ms_replace_env.c \
		ms_env.c \
		ms_unset.c \
		ms_line_token1.c \
		ms_line_token2.c \
		ms_line_token_utils.c \
		ms_rdr_input1.c \
		ms_rdr_input2.c \
		ms_rdr_output1.c \
		ms_rdr_output2.c \
		ms_pipe_split.c \
		ms_pipe_exec.c \
		ms_builtin_func.c \
		ms_exit.c \
		ms_exit_num_check.c \
		ms_export.c \
		ms_cd.c \
		ms_cd_controller.c

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
