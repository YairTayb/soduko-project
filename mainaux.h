#ifndef SUDOKU_MAIN_AUX_H
#define SUDOKU_MAIN_AUX_H

#include "data_models.h"
#include "consts.h"


void print_board(board grid, int grid_height, int grid_width, int box_height, int box_width,
                 game_mode mode, int mark_errors);

board create_empty_board(int grid_height, int grid_width);

void copy_board(board source_grid, board destination_grid, int grid_height, int grid_width);

void empty_board(board board_to_empty, int grid_height, int grid_width);

void free_board(board grid, int grid_height);

int is_board_errornous(board board, int grid_height, int grid_width);

int is_valid_input(int num, int max_num_in_range);

void print_invalid_value(int lower_limit, int upper_limit);

void print_fixed_cell_error(int row, int col);

void print_cell_is_not_empty(int row, int col);

void print_winning_message();

void print_errornous_board_message();

void print_hint_message(int row, int col, int hint_value);

void print_validation_failed();

void print_validation_passed();

void print_num_of_solutions(int solutions_count);

void handle_errors(returnCodeDesc return_code_desc);

int is_error(returnCodeDesc return_code_desc);

#endif

