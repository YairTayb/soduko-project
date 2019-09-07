#ifndef SUDOKU_PARSER_H
#define SUDOKU_PARSER_H

#include "data_models.h"
#include "consts.h"

returnCodeDesc parse_double(char *num, double *res);

returnCodeDesc read_board_from_file(FILE *fd, board *grid_pointer, int *grid_height_pointer, int *grid_width,
                         int *box_height_pointer,
                         int *box_width_pointer);

returnCodeDesc write_board_to_file(board grid, int grid_height, int grid_width, int box_width, int box_height, FILE *fd,
                        game_mode mode_of_game);

returnCodeDesc parse_command(command *user_command);

returnCodeDesc validate_amount_of_parameters(command user_command);

returnCodeDesc validate_command_mode(command user_command, game_mode current_mode);

#endif
