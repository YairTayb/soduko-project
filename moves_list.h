#define SUCCESS 1
#define FAILURE 0

struct Node  {
	struct Cell** data;
	struct Node* next;
	struct Node* prev;
};

struct List_of_moves
{
	struct Node* current_move;
    int board_width;
    int board_height;
};

struct Cell** psuedo_undo(struct List_of_moves*  list);
struct Cell** psuedo_redo(struct List_of_moves*  list);
void psuedo_reset(struct List_of_moves*  list);
void free_whole_list(struct List_of_moves*  list);
void add_move (struct Cell** grid, int grid_height, int grid_width, struct List_of_moves*  list);