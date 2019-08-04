#ifndef NIM_GAME_H
#define NIM_GAME_H

#include "mainaux.h"

#define GAME_WON 1
#define SET_COMPLETED 0
#define SET_INCOMPLETE -1
#define ERRORNOUS_BOARD -2

typedef enum Mode{
    init, edit, solve
} game_mode;

typedef enum CommandFrom{
    endo,redo, reset
}command_changed_from;

int set(struct Cell **grid, int grid_height, int grid_width, int box_height, int box_width,
        int row, int col, int value, game_mode mode, int mark_errors);


#endif
