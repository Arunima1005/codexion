NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

INCLUDES = -Iinclude

SRC = src/main.c \
	src/time_utils.c \
	src/dongle_utils.c \
      src/utils.c \
	  src/parse.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re