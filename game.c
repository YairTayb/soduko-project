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
 * @return 1 = Game was won, 0 = Set was successful, -1 = Error, -2 = Board is errornous.
 */
int set(struct Cell **grid, int grid_height, int grid_width, int box_height, int box_width,
        int row, int col, int value, game_mode mode, int mark_errors) {
    /* Validate the input */
    if (!is_valid_input(row, grid_height - 1)) {
        print_invalid_value(1, grid_height);
        return COMMAND_INCOMPLETE;
    }

    if (!is_valid_input(col, grid_width - 1)) {
        print_invalid_value(1, grid_width);
        return COMMAND_INCOMPLETE;
    }

    if (!is_valid_input(value, (box_height * box_width))) {
        print_invalid_value(0, (box_height * box_width));
        return COMMAND_INCOMPLETE;
    }

    /* Validate the cell is not fixed - if it is and we are in solve mode -
     * print error and don't perform command */
    if (mode == solve && grid[row][col].is_const == TRUE) {
        print_fixed_cell_error(row, col);
        return COMMAND_INCOMPLETE;
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

    return COMMAND_COMPLETED;
}
void print_changes(struct Cell **before, struct Cell **after, int grid_height, int grid_width, command_changed_from comm){

    int i, j;
    for(i = 0;i < grid_height; i++){
        for(j = 0; j < grid_width; j++){
            /*check if the value has changed*/
            if(before[i][j].value != after[i][j].value){

                if(comm == undo){
                    printf("Undo ");
                } else if(comm == redo){
                    printf("Redo ");
                } 
                printf("%d,%d from %d to %d .\n",i,j, before[i][j].value , after[i][j].value);

            }
        }
    }
}


int undo(struct Cell **grid, int grid_height, int grid_width, struct List_of_moves *list) {

    struct Node* temp = psuedo_undo(list);
    if (temp == NULL) {
        /*ERROR HANDLING - no undo available*/
        return FAILURE;
    } else {
        print_changes(grid,temp->data,grid_height,grid_width,undo);
        copy_board(temp->data, grid, grid_height, grid_width);
        return SUCCESS;
    }
}

int redo(struct Cell **grid, int grid_height, int grid_width, struct List_of_moves *list) {

    struct Node*temp = psuedo_redo(list);
    if (temp == NULL) {
        /*ERROR HANDLING - no redo available*/
        return FAILURE;
    } else {
        print_changes(grid,temp->data,grid_height,grid_width,redo);
        copy_board(temp->data, grid, grid_height, grid_width);
        return SUCCESS;
    }
}

void restart_game(struct Cell **grid, int grid_height, int grid_width, struct List_of_moves *list) {
    struct Node* temp = psuedo_reset(list);
    copy_board(temp->data, grid, list->board_height, list->board_width);
}


char *parse_board(struct Cell **grid, int grid_height, int grid_width, int box_height, int box_width) {


}


/**
 * Parsing the board into a string.
 * 
 * 
*/
int save(struct Cell **grid, int grid_height, int grid_width, int box_height, int box_width, game_mode mode, char* path) {

    int i, j, tok;
    FILE* fd;
    struct Cell **new_solution = create_empty_board(GRID_HEIGHT, GRID_WIDTH);
    copy_board(grid, new_solution, grid_height, grid_width);


    if (mode == edit) {
        if ( is_board_errornous(grid,grid_height, grid_width)){
            /*ERROR HANDLING*/
            free_board(new_solution,grid_height);
            return ERRORNOUS_BOARD;
        }
        if (!solve_grid(new_solution,grid_height, grid_width,box_height,box_width,0,0) ){
            /*ERROR HANDLING*/
            print_validation_failed();
            free_board(new_solution,grid_height);
            return COMMAND_INCOMPLETE;
        }
    } 
    fd = fopen(path, "w");
    if (!fd){
        /*ERROR HANLING*/
        return COMMAND_INCOMPLETE;
    }
    tok = write_board_to_file(grid, grid_height, grid_width, box_width, box_height, fd, mode);
    if(tok == FAILURE){
        /*ERROR HANDLING- HOW TO TREAT?*/
        return COMMAND_INCOMPLETE;
    } 
    return COMMAND_COMPLETED;
}


/**
 * Perform validate command.
 * @param grid The game board
 * @param grid_height The height of game board
 * @param grid_width The width of the game board
 * @param box_height The height of a sudoku box
 * @param box_width The width of a sudoku box
 * @return 1 = Validation successful, 0 = No solution found, -2 = Board is errornous, command was not executed.
 */
int validate(struct Cell **grid, int grid_height, int grid_width, int box_height, int box_width) {
    /* Create a temporary board for storing the new solution */
    struct Cell **new_solution = create_empty_board(GRID_HEIGHT, GRID_WIDTH);
    copy_board(grid, new_solution, grid_height, grid_width);

    if (is_board_errornous(grid, grid_height, grid_width)) {
        print_errornous_board_message();
        free_board(new_solution);
        return ERRORNOUS_BOARD;
    }


    /* If board is solvable - update the solution */
    if (solve_grid(new_solution, grid_height, grid_width, box_height, box_width, 0, 0) == TRUE) {
        print_validation_passed();
        /* Free memory allocation for previous solution */
        free_board(new_solution, grid_height);
        return TRUE;

    } else {
        /* No solution for the current board */
        /* Free memory allocation for previous solution */
        free_board(new_solution, grid_height);
        print_validation_failed();
        return FALSE;
    }
}


/**
 * Count number of possible solutions for the current board
 * @param grid The game board
 * @param grid_height The height of game board
 * @param grid_width The width of the game board
 * @param box_height The height of a sudoku box
 * @param box_width The width of a sudoku box
 */
void num_solutions(struct Cell **grid, int grid_height, int grid_width, int box_height, int box_width){
    int solutions_count;

    if (is_board_errornous(grid, grid_height, grid_width)) {
        print_errornous_board_message();
        return ERRORNOUS_BOARD;
    }

    solutions_count = count_solutions_iterative(grid, grid_height, grid_width, box_height, box_width);
    print_num_of_solutions(solutions_count);
}

int autofill(struct Cell **grid, int grid_height, int grid_width, int box_height, int box_width) {
    int i;
    int j;
    int num;

    struct Cell **temp_board = create_empty_board(GRID_HEIGHT, GRID_WIDTH);
    copy_board(grid, temp_board, grid_height, grid_width);

    for (i=0; i<grid_height; i++){
        for (j=0; j<grid_width; j++){
            if (is_empty(grid, row, col) && count_valid_values(grid, grid_height, grid_width, box_height, box_width) == 1){
                /* Find the one possible valid value for the current cell */
                for (num = 1; num < (box_height * box_width); num++){
                    if (is_valid(grid, grid_height, grid_width, box_height, box_width, row, col, num)) {
                        /* Set the cell */
                        temp_board[row][col].value = num;
                        break;
                    }
                }
            }
        }
    }

    copy_board(temp_board, grid, grid_height, grid_width);
    free_board(temp_board, grid_height);

    /* Update the board errors */
    update_board_errors(grid, grid_height, grid_width, box_height, box_width);

}

int hint(struct Cell **grid, int grid_height, int grid_width, int box_height, int box_width, int row, int col) {
    /* Create a temporary board for storing the new solution */
    struct Cell **new_solution = create_empty_board(GRID_HEIGHT, GRID_WIDTH);
    copy_board(grid, new_solution, grid_height, grid_width);

    if (is_board_errornous(grid, grid_height, grid_width)) {
        print_errornous_board_message();
        free_board(new_solution, grid_height);
        return ERRORNOUS_BOARD;
    }

    /* Validate the cell is not fixed - if it is print an error and don't perform command */
    if (grid[row][col].is_const == TRUE) {
        print_fixed_cell_error(row, col);
        free_board(new_solution, grid_height);
        return COMMAND_INCOMPLETE;
    }

    /* Validate the cell is empty - if it isn't print an error and don't perform command */
    if (!is_empty(grid, row, col)){
        print_cell_is_not_empty(row, col);
        free_board(new_solution, grid_height);
        return COMMAND_INCOMPLETE;
    }

    if (solve_grid(new_solution, grid_height, grid_width, box_height, box_width, 0, 0) == TRUE) {
        print_hint_message(row, col, new_solution[row][col].value);
        /* Free memory allocation for previous solution */
        free_board(new_solution, grid_height);
        return COMMAND_COMPLETED;

    } else {
        /* No solution for the current board */
        /* Free memory allocation for previous solution */
        free_board(new_solution, grid_height);
        print_validation_failed();
        return COMMAND_INCOMPLETE;
    }

}
