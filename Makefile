EXECUTABLE = ft_ls
CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic
REMOVE = rm -f
LS_UTILS = lib/ls_utils.a

BLACK = \033[0;30m
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
PURPLE = \033[0;35m
CYAN = \033[0;36m
WHITE = \033[0;37m
MAGENTA = \e[95m

SRC =	./src/main.c
OBJS = ${SRC:.c=.o}

all: lib $(EXECUTABLE)

$(EXECUTABLE): $(OBJS) $(LS_UTILS)
	$(CC) $(OBJS) $(LS_UTILS) -o $(EXECUTABLE)

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $^

lib:
	@make -s -C lib/

clean:
	@make clean -s -C lib/
	$(REMOVE) $(OBJS)

fclean: clean
	@make fclean -s -C lib/
	$(REMOVE) $(EXECUTABLE)

re: fclean all

.PHONY: lib all clean fclean re