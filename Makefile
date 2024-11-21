CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude
LDFLAGS = -lreadline

SRCDIR = src
EXCLUDE_DIRS = random_files garbage_collector
SRC = $(shell find $(SRCDIR) -type f -name '*.c' $(foreach dir,$(EXCLUDE_DIRS),! -path '*/$(dir)/*'))
OBJ = $(SRC:.c=.o)
NAME = minishell

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
