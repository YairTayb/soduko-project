#ifndef NIM_GAME_H
#define NIM_GAME_H

#include "mainaux.h"

#define GAME_WON 1
#define COMMAND_COMPLETED 0
#define COMMAND_INCOMPLETE -1
#define ERRORNOUS_BOARD -2

typedef enum Mode{
    init, edit, solve
} game_mode;


int set(struct Cell **grid, int grid_height, int grid_width, int box_height, int box_width,
        int row, int col, int value, game_mode mode, int mark_errors);


#endif
