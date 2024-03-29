/*Solver module.
This module consists of all the methods used in order to solve the board.
The methods give indication about board statuses according to demand of other methods
used in the solving procedure of the game.*/

#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H

#include "data_models.h"
#include "consts.h"

int is_valid(board grid, int grid_height, int grid_width, int box_height, int box_width,
             int row, int col, int num);

int generate_board(board grid, board solution, int grid_height, int grid_width, int box_height,
                   int box_width, int num_of_hints);

int is_board_complete(board grid, int grid_height, int grid_width);

void update_board_errors(board board, int grid_height, int grid_width, int box_height, int box_width);

int count_solutions_iterative(board grid, int grid_height, int grid_width, int box_height, int box_width);

int
count_valid_values(board grid, int grid_height, int grid_width, int box_height, int box_width, int row, int col,
                   int range);

int is_cell_errornous(board grid, int row, int col);

int is_empty(board grid, int row, int col);

int count_empty_cells(board board, int grid_height, int grid_width);

int is_board_valid(board game_board, int grid_height, int grid_width, int box_height, int box_width);

int find_valid_values(int *values, board game_board, int grid_height, int grid_width, int box_height, int box_width,
                      int row, int col, int range);

void reset_board_changed_status(board game_board, int grid_height, int grid_width);

#endif
