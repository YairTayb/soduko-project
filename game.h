#ifndef NIM_GAME_H
#define NIM_GAME_H


#define FAILURE -1
#define GAME_WON 1
#define COMMAND_COMPLETED 0
#define COMMAND_INCOMPLETE -1
#define ERRORNOUS_BOARD -2

#include "mainaux.h"


struct Node  {
	struct Cell** data;
	struct Node* next;
	struct Node* prev;
};

struct List_of_moves
{
	struct Node* current_move;
    int board_width;
    int board_height;
};

typedef enum CommandFrom{
    undo,redo, reset
}command_changed_from;

int set(struct Cell **grid, int grid_height, int grid_width, int box_height, int box_width,
        int row, int col, int value, game_mode mode, int mark_errors);
int solve(struct Cell ***grid_pointer, char *path, int *grid_height_pointer, int *grid_width_pointer,
                    int *box_height_pointer,
                    int *box_width_pointer);
int edit(struct Cell ***grid_pointer, char *path, int *grid_height_pointer, int *grid_width_pointer,
                    int *box_height_pointer,
                    int *box_width_pointer);
int redo_move(struct Cell **grid, int grid_height, int grid_width, struct List_of_moves *list);
int undo_move(struct Cell **grid, int grid_height, int grid_width, struct List_of_moves *list);

#endif
