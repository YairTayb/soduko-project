#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdlib.h>
#include "mainaux.h"

/**
 * Print the current board to the user
 * @param grid The game board
 * @param grid_height The height of game board
 * @param grid_width The width of the game board
 * @param box_height The height of a sudoku box
 * @param box_width The width of a sudoku box
 */
void print_board(struct Cell **grid, int grid_height, int grid_width, int box_height, int box_width) {
    int i, j, k, line_length, boxes_amount;
    boxes_amount = grid_width / box_width;

    /* The formula for lines given by Moshe. */
    line_length = boxes_amount * (CHARACTERS_IN_CELL * box_width + 2) + 1;

    for (i = 0; i < grid_height; i++) {
        /* Printing the horizontal lines */

        if (i % box_height == 0) {
            for (k = 0; k < line_length; k++) {
                printf("-");
            }
            printf("\n");
        }
        for (j = 0; j < grid_width; j++) {
            if (j == 0) {
                /* the box frame */
                printf("| ");
            } else if (j % box_width == 0) {
                printf("| ");
            }

            if (grid[i][j].value == UNASSIGNED)
                printf(EMPTY_CELL);

            else if (grid[i][j].is_const == FALSE) {
                printf(NORMAL_CELL, grid[i][j].value);
            } else {
                /* Its is a constant value */
                printf(CONST_CELL, grid[i][j].value);
            }


            if (j == grid_width - 1) {
                /* The box frame */
                printf("|\n");
            }

        }
    }

    /* One last horizontal line. */
    for (k = 0; k < line_length; k++) {
        printf("-");
    }
    printf("\n");
}


/**
 * Copy a board
 * @param source_grid The board to copy from
 * @param destination_grid The board to copy to
 * @param grid_height The height of the board
 * @param grid_width The width of the board
 */
void copy_board(struct Cell **source_grid, struct Cell **destination_grid, int grid_height, int grid_width) {
    int i;
    int j;
    for (i = 0; i < grid_height; i++) {
        for (j = 0; j < grid_width; j++) {
            destination_grid[i][j].value = source_grid[i][j].value;
            destination_grid[i][j].is_const = source_grid[i][j].is_const;
        }
    }
}

/**
 * Create an empty board
 * @param grid_height The board height
 * @param grid_width The board width
 * @return
 */
struct Cell **create_empty_board(int grid_height, int grid_width) {
    struct Cell **grid = (struct Cell **) malloc(grid_height * grid_width * sizeof(struct Cell));
    int i;
    int j;

    /*check if malloc failed*/
    if(!grid){
        printf(FUNCTION_FAILED, "malloc");
        exit(0);
    }
    for (i = 0; i < grid_height; i++){
        grid[i] = (struct Cell *) malloc(grid_width * sizeof(struct Cell));
        /*check if malloc failed*/
        if(!grid[i]){
            printf(FUNCTION_FAILED, "malloc");
            exit(0);
        }
        
    }
        
    /* Initiate the board */
    for (i = 0; i < grid_height; i++) {
        for (j = 0; j < grid_width; j++) {
            struct Cell cell;
            cell.value = UNASSIGNED;
            cell.is_const = FALSE;
            grid[i][j] = cell;
        }
    }

    return grid;
}

/**
 * Empty a given board
 * @param board_to_empty The board to empty
 * @param grid_height The board height
 * @param grid_width The board width
 */
void empty_board(struct Cell **board_to_empty, int grid_height, int grid_width) {
    int i, j;
    for (i = 0; i < grid_height; i++) {
        for (j = 0; j < grid_width; j++) {
            board_to_empty[i][j].value = UNASSIGNED;
            board_to_empty[i][j].is_const = FALSE;
        }
    }
}

void free_board(struct Cell** grid, int grid_height){
    int i;
    for ( i=0 ; i < grid_height; i++ ){
        free (grid[i]);
    }
    free(grid);
}