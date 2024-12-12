NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBFT = ./libft/libft.a
SRCS = src/minishell.c src/parser.c src/lexer.c src/executor.c src/env.c src/signals.c src/utils.c \
       src/expands.c src/builtins.c \
       src/builtins/builtin_cd.c src/builtins/builtin_echo.c src/builtins/builtin_env.c src/builtins/builtin_exit.c \
       src/builtins/builtin_export.c src/builtins/builtin_pwd.c src/builtins/builtin_unset.c
INCS = -I includes -I libft
RLFLAGS = -lreadline

all: $(NAME)

$(NAME): $(SRCS) $(LIBFT)
	$(CC) $(CFLAGS) $(INCS) $(SRCS) $(LIBFT) $(RLFLAGS) -o $(NAME)

$(LIBFT):
	make -C libft

clean:
	make clean -C libft

fclean: clean
	make fclean -C libft
	rm -f $(NAME)

re: fclean all
