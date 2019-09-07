#ifndef NIM_GAME_H
#define NIM_GAME_H

#include "data_models.h"

returnCodeDesc set(board grid, int grid_height, int grid_width, int box_height, int box_width,
        int row, int col, int value, game_mode mode);

returnCodeDesc solve(board *grid_pointer, char *path, int *grid_height_pointer, int *grid_width_pointer,
          int *box_height_pointer,
          int *box_width_pointer);

returnCodeDesc edit(board *grid_pointer, char *path, int *grid_height_pointer, int *grid_width_pointer,
         int *box_height_pointer,
         int *box_width_pointer,
         int path_was_passed);

returnCodeDesc redo_move(board grid, int grid_height, int grid_width, struct MovesList *list);

returnCodeDesc undo_move(board grid, int grid_height, int grid_width, struct MovesList *list);

returnCodeDesc save(board game_board, int grid_height, int grid_width, int box_height, int box_width, game_mode mode, char* path);

returnCodeDesc validate(board game_board, int grid_height, int grid_width, int box_height, int box_width);

returnCodeDesc hint(board game_board, int grid_height, int grid_width, int box_height, int box_width, int row, int col);

returnCodeDesc generate(board game_board, int grid_height, int grid_width, int box_height, int box_width,
                        int num_of_cells_to_fill, int num_of_cells_to_keep);

returnCodeDesc autofill(board game_board, int grid_height, int grid_width, int box_height, int box_width);

returnCodeDesc num_solutions(board game_board, int grid_height, int grid_width, int box_height, int box_width);

returnCodeDesc reset_game(board game_board, struct MovesList *list);

returnCodeDesc set_mark_errors(int* mark_errors, int input);

returnCodeDesc guess(board game_board, int grid_height, int grid_width, int box_height, int box_width, double threshold);

returnCodeDesc guess_hint(board game_board, int grid_height, int grid_width, int box_height, int box_width, int row, int col);

#endif
