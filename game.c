#include "game.h"
#include "solver.h"
#include "moves_list.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * Perform set command. Set a value on a given cell.
 * @param grid The game board
 * @param grid_height The height of game board
 * @param grid_width The width of the game board
 * @param box_height The height of a sudoku box
 * @param box_width The width of a sudoku box
 * @param col The column of the cell to set
 * @param row The width of the cell to set
 * @param value The value to set to the cell
 * @return 1 = Game was won, 0 = Set was successful, -1 = Error.
 */
int set(struct Cell **grid, int grid_height, int grid_width, int box_height, int box_width,
        int row, int col, int value, game_mode mode, int mark_errors) {
    /* Validate the input */
    if (!is_valid_input(value, (box_height * box_width))) {
        print_invalid_value((box_height * box_width));
        return SET_INCOMPLETE;
    }

    /* Validate the cell is not fixed - if it is and we are in solve mode -
     * print error and don't perform command */
    if (mode == solve && grid[row][col].is_const == TRUE) {
        print_fixed_cell_error(row, col);
        return SET_INCOMPLETE;
    }

    /* Set the cell */
    grid[row][col].value = value;
    update_board_errors(grid, grid_height, grid_width, box_height, box_width);

    print_board(grid, GRID_WIDTH, GRID_HEIGHT, BOX_WIDTH, BOX_HEIGHT);

    /* Check if the game was won */
    if (is_board_complete(grid, grid_height, grid_width)) {
        /* Board is fully filled */
        if (is_board_errornous(grid, grid_height, grid_width)) {
            /* Board contains errors - notify user */
            print_errornous_board_message();
            return ERRORNOUS_BOARD;

        } else {
            /* Board is completed and valid - game was won */
            print_winning_message();
            return GAME_WON;
        }
    }

    return SET_COMPLETED;

}

int undo(struct Cell **grid, int grid_height, int grid_width, struct List_of_moves *list) {

    struct Cell **temp = psuedo_undo(list);
    if (temp == NULL) {
        /*ERROR HANDLING - no undo available*/
        return FAILURE;
    } else {
        copy_board(temp, grid, grid_height, grid_width);
        return SUCCESS;
    }
}

int redo(struct Cell **grid, int grid_height, int grid_width, struct List_of_moves *list) {

    struct Cell **temp = psuedo_redo(list);
    if (temp == NULL) {
        /*ERROR HANDLING - no redo available*/
        return FAILURE;
    } else {
        copy_board(temp, grid, grid_height, grid_width);
        return SUCCESS;
    }
}

void restart_game(struct Cell **grid, int grid_height, int grid_width, struct List_of_moves *list) {
    struct Cell **temp = psuedo_reset(list);
    copy_board(temp, grid, list->board_height, list->board_width);
}


char *parse_board(struct Cell **grid, int grid_height, int grid_width, int box_height, int box_width) {


}


/**
 * Parsing the board into a string.
 * 
 * 
*/
char *save(struct Cell **grid, int grid_height, int grid_width, int mode) {

    int i, j;

    if (mode == edit_mode) {
        /*tests for board status*/
    } else if (mode == solve_mode) {

    }
}







