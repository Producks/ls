NAME = ls_utils.a
CC = gcc
CFGLAGS = -Wall -Werror -Wextra -pedantic -O2
REMOVE = rm -f
LIBC = ar rcs
SRC =	f_memset.c f_strlen.c f_write_fd.c f_strdup.c f_strncpy.c f_strcpy.c f_strcmp.c f_separator.c
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