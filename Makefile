CC = clang
CFLAGS = -Wall -Wextra -Werror -Iinclude -I./libft -fsanitize=address,leak,undefined -g3 -O0
LDFLAGS = -lreadline -L./libft -lft

SRCDIR = src
EXCLUDE_DIRS = random_files garbage_collector
SRC = $(shell find $(SRCDIR) -type f -name '*.c' $(foreach dir,$(EXCLUDE_DIRS),! -path '*/$(dir)/*'))
OBJ = $(SRC:.c=.o)
NAME = minishell

all: $(NAME)

$(NAME): $(OBJ)
	@make -C ./libft
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@make clean -C ./libft
	rm -f $(OBJ)

fclean: clean
	@make fclean -C ./libft
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
