#ifndef NIM_GAME_H
#define NIM_GAME_H

#include "mainaux.h"

typedef enum Mode{
    init, edit, solve
} game_mode;


int set(struct Cell **grid, int grid_height, int grid_width, int box_height, int box_width,
        int row, int col, int value);


#endif
