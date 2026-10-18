NAME = cunix

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude

SRC = src/main.c \
      src/cat.c \
      src/tail.c \
      src/utils.c \
	  src/grep.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re