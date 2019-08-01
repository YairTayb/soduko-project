#include "game.h"
#include "moves_list.h"

#include <stdlib.h>
#include <stdio.h>



int undo ( struct Cell **grid, int grid_height, int grid_width, struct List_of_moves* list){

    struct Cell** temp = psuedo_undo(list);
    if(temp == NULL){
        /*ERROR HANDLING - no undo available*/
        return FAILURE;
    } else{
        copy_board(temp, grid, grid_height, grid_width);
        return SUCCESS;
    }
}

int redo( struct Cell **grid, int grid_height, int grid_width, struct List_of_moves* list){

    struct Cell** temp = psuedo_redo(list);
    if(temp == NULL){
        /*ERROR HANDLING - no redo available*/
        return FAILURE;
    } else{
        copy_board(temp, grid, grid_height, grid_width);
        return SUCCESS;
    }
}

void restart_game( struct Cell **grid, int grid_height, int grid_width, struct List_of_moves* list){
    struct Cell** temp = psuedo_reset(list);
    copy_board(temp,grid, list->board_height ,list->board_width);
}


char * parse_board( struct Cell **grid, int grid_height, int grid_width, int box_height, int box_width){
    



}


/**
 * Parsing the board into a string.
 * 
 * 
*/
char* save ( struct Cell **grid, int grid_height, int grid_width, int mode){
    
    int i,j;

    if (mode == edit_mode){
        /*tests for board status*/
    } else if(mode == solve_mode){

    }
}







