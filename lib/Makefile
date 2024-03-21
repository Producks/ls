NAME = ls_utils.a
CC = gcc
CFGLAGS = -Wall -Werror -Wextra
REMOVE = rm -f
LIBC = ar rcs

BLACK = \033[0;30m
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
PURPLE = \033[0;35m
CYAN = \033[0;36m
WHITE = \033[0;37m

SRC =	f_memset.c
OBJS = $(SRC:.c=.o)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $^

all:$(NAME)

clean:
	$(REMOVE) $(OBJS)

fclean:clean
	$(REMOVE) $(NAME)

re:fclean all

.PHONY: all clean fclean re