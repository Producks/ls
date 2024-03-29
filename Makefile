EXECUTABLE = ft_ls
CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -O2
REMOVE = rm -f
LS_UTILS = lib/ls_utils.a
SRC =	./src/main.c ./src/ls.c ./src/parser.c ./src/traversal.c ./src/queue.c ./src/sorting.c ./src/format.c ./src/utils.c
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