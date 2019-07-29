#include "game.h"
#include "moves_list.h"

#include <stdlib.h>
#include <stdio.h>



int undo ( struct Cell **grid, int grid_height, int grid_width, struct List_of_moves* list){
    
    struct Cell** temp = psuedo_undo(list);
    copy_board(temp, grid, grid_height, grid_width);
}

int main(){

}







