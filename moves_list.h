#define SUCCESS 1
#include "game.h"



struct Node* psuedo_undo(struct List_of_moves*  list);
struct Node* psuedo_redo(struct List_of_moves*  list);
struct Node* psuedo_reset(struct List_of_moves*  list);
void free_whole_list(struct List_of_moves*  list);
void add_move (struct Cell** grid, int grid_height, int grid_width, struct List_of_moves*  list);
void restart_list(struct List_of_moves*  list);