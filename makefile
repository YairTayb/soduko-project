CC = gcc
OBJS = stack.o mainaux.o
EXEC = sudoku-project
COMP_FLAG = -ansi -Wall -Wextra -Werror -pedantic-errors

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
stack.o: stack.h mainaux.h
	$(CC) $(COMP_FLAG) -c $*.c
mainaux.o: mainaux.c mainaux.h
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
