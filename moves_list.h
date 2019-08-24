#ifndef SUDOKU_MOVES_LIST_H
#define SUDOKU_MOVES_LIST_H

#include "data_models.h"
#include "consts.h"

struct Node *psuedo_undo(struct MovesList *list);

struct Node *psuedo_redo(struct MovesList *list);

struct Node *psuedo_reset(struct MovesList *list);

void free_whole_list(struct MovesList *list);

void add_move_to_list(board grid, struct MovesList *list);

void restart_list(struct MovesList *list);

void init_move_list(struct MovesList*  list, int grid_height, int grid_width);

#endif
