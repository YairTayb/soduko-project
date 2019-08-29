CC = gcc
OBJS = main.o game.o parser.o solver.o mainaux.o moves_list.o stack.o
EXEC = sudoku-console
COMP_FLAG = -ansi -Wall -Wextra -Werror -pedantic-errors

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
main.o: main.c mainaux.h parser.h game.h consts.h data_models.h
	$(CC) $(COMP_FLAG) -c $*.c
game.o: game.c game.h mainaux.h solver.h moves_list.h parser.h consts.h data_models.h
	$(CC) $(COMP_FLAG) -c $*.c
parser.o: parser.c parser.h mainaux.h consts.h data_models.h
	$(CC) $(COMP_FLAG) -c $*.c
solver.o: solver.c stack.h solver.h consts.h data_models.h
	$(CC) $(COMP_FLAG) -c $*.c
stack.o: stack.c stack.h mainaux.h consts.h data_models.h
	$(CC) $(COMP_FLAG) -c $*.c
moves_list.o: moves_list.c moves_list.h mainaux.h consts.h data_models.h
	$(CC) $(COMP_FLAG) -c $*.c
mainaux.o: mainaux.c mainaux.h consts.h data_models.h
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
