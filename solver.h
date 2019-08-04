#include "mainaux.h"

#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H

#define RANDOMLY 1
#define DETERMINISTIC 2


int is_valid(struct Cell **grid, int grid_height, int grid_width, int box_height, int box_width,
             int row, int col, int num);

int generate_board(struct Cell **grid, struct Cell **solution, int grid_height, int grid_width, int box_height,
                   int box_width, int num_of_hints);

int is_board_complete(struct Cell **grid, int grid_height, int grid_width);

void update_board_errors(struct Cell **board, int grid_height, int grid_width, int box_height, int box_width);

int count_solutions_iterative(struct Cell **grid, int grid_height, int grid_width, int box_height, int box_width);

int count_valid_values(struct Cell **grid, int grid_height, int grid_width, int box_height, int box_width, int row, int col, int range);

int is_cell_errornous(struct Cell **grid, int row, int col);

int is_empty(struct Cell **grid, int row, int col);

#endif
