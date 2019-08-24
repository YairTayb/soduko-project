#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "game.h"
#include "mainaux.h"
#include "solver.h"
#include "moves_list.h"
#include "parser.h"
#include "consts.h"
#include "data_models.h"

/**
 * Perform set command. Set a value on a given cell.
 * @param game_board The game board
 * @param grid_height The height of game board
 * @param grid_width The width of the game board
 * @param box_height The height of a sudoku box
 * @param box_width The width of a sudoku box
 * @param col The column of the cell to set
 * @param row The width of the cell to set
 * @param value The value to set to the cell
 * @return 1 = Game was won, 0 = Set was successful, -1 = Error, -2 = Board is errornous.
 */
returnCodeDesc set(board game_board, int grid_height, int grid_width, int box_height, int box_width,
        int row, int col, int value, game_mode mode) {
    returnCodeDesc return_code_desc;

    /* Validate the input */
    if (!is_valid_input(row, grid_height - 1)) {
        return_code_desc.error_code = E_INVALID_VALUE;
        sprintf(return_code_desc.error_message, INVALID_VALUE_ERROR, 1, grid_height);
        return return_code_desc;
    }

    if (!is_valid_input(col, grid_width - 1)) {
        return_code_desc.error_code = E_INVALID_VALUE;
        sprintf(return_code_desc.error_message, INVALID_VALUE_ERROR, 1, grid_height);
        return return_code_desc;
    }

    if (!is_valid_input(value, (box_height * box_width))) {
        return_code_desc.error_code = E_INVALID_VALUE;
        sprintf(return_code_desc.error_message, INVALID_VALUE_ERROR, 0, (box_height * box_width));
        return return_code_desc;
    }

    /* Validate the cell is not fixed - if it is and we are in solve mode -
     * print error and don't perform command */
    if (mode == solve_mode && game_board[row][col].is_const == TRUE) {
        return_code_desc.error_code = E_CELL_IS_FIXED;
        sprintf(return_code_desc.error_message, CELL_IS_FIXED_ERROR, row, col);
        return return_code_desc;
    }

    /* Set the cell */
    game_board[row][col].value = value;

    return_code_desc.error_code = E_SUCCESS;
    strcpy(return_code_desc.error_message, NO_ERRORS);
    return return_code_desc;

}
void print_changes(board before, board after, int grid_height, int grid_width, command_changed_from comm){

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


returnCodeDesc undo_move(board game_board, int grid_height, int grid_width, struct MovesList *list) {

    struct Node* temp = psuedo_undo(list);
    returnCodeDesc return_value;
    if (temp == NULL) {
        strcpy(return_value.error_message, CANNOT_UNDO);
        return_value.error_code = E_CANNOT_UNDO;
        return return_value;
    } else {
        print_changes(game_board,temp->data,grid_height,grid_width,undo);
        copy_board(temp->data, game_board, grid_height, grid_width);
        return_value.error_code = E_SUCCESS;
        return return_value;
    }
}

returnCodeDesc redo_move(board game_board, int grid_height, int grid_width, struct MovesList *list) {

    struct Node*temp = psuedo_redo(list);
    returnCodeDesc return_value;

    if (temp == NULL) {
        strcpy(return_value.error_message, CANNOT_REDO);
        return_value.error_code = E_CANNOT_REDO;
        return return_value;
    } else {
        print_changes(game_board,temp->data,grid_height,grid_width,redo);
        copy_board(temp->data, game_board, grid_height, grid_width);
        return_value.error_code = E_SUCCESS;
        strcpy(return_value.error_message, NO_ERRORS);
        return return_value;
    }
}


returnCodeDesc reset_game(board game_board, struct MovesList *list) {
    returnCodeDesc return_code_desc;
    struct Node* temp = psuedo_reset(list);
    copy_board(temp->data, game_board, list->board_height, list->board_width);

    return_code_desc.error_code = E_SUCCESS;
    strcpy(return_code_desc.error_message, NO_ERRORS);
    return return_code_desc;
}





/**
 * Parsing the board into a string.
 * 
 * 
*/
returnCodeDesc save(board game_board, int grid_height, int grid_width, int box_height, int box_width, game_mode mode, char* path) {

    FILE *fd;
    returnCodeDesc return_code_desc;

    board board_copy = create_empty_board(grid_height, grid_width);
    copy_board(game_board, board_copy, grid_height, grid_width);

    if (mode == edit_mode) {
        if (is_board_errornous(game_board, grid_height, grid_width)) {
            return_code_desc.error_code = E_ERRORNOUS_BOARD;
            strcpy(return_code_desc.error_message, ERROR_BOARD_MSG);
            return return_code_desc;
        }

        if (!solve_grid(board_copy, grid_height, grid_width, box_height, box_width, 0, 0)) {
            return_code_desc.error_code = E_NO_SOLUTION;
            strcpy(return_code_desc.error_message, VALIDATION_FAILED);
            return return_code_desc;
        }
    }

    fd = fopen(path, "w");

    if (!fd) {
        return_code_desc.error_code = E_OPEN_FILE_FAILED;
        sprintf(return_code_desc.error_message, FAILED_OPENING_FILE, path);

    } else {
        /* TODO - Maybe write_board_to_file shouldn't get mode, and all the narking of fixed/non fixed should happen here? */
        return_code_desc = write_board_to_file(game_board, grid_height, grid_width, box_width, box_height, fd, mode);
    }

    if (fclose(fd) != 0) {
        /* Error handling */
        return_code_desc.error_code = E_READ_FROM_FILE_FAILED;
        sprintf(return_code_desc.error_message, FUNCTION_FAILED, "fclose");
        return return_code_desc;
    }

    return return_code_desc;

}


/**
 * Perform validate command.
 * @param game_board The game board
 * @param grid_height The height of game board
 * @param grid_width The width of the game board
 * @param box_height The height of a sudoku box
 * @param box_width The width of a sudoku box
 * @return 1 = Validation successful, 0 = No solution found, -2 = Board is errornous, command was not executed.
 */
returnCodeDesc validate(board game_board, int grid_height, int grid_width, int box_height, int box_width) {
    returnCodeDesc return_code_desc;

    /* Create a temporary board for storing the new solution */
    board new_solution = create_empty_board(grid_height, grid_width);
    copy_board(game_board, new_solution, grid_height, grid_width);

    if (is_board_errornous(game_board, grid_height, grid_width)) {
        return_code_desc.error_code = E_ERRORNOUS_BOARD;
        strcpy(return_code_desc.error_message, ERROR_BOARD_MSG);
    }

    /* If board is solvable - update the solution */
    else if (solve_grid(new_solution, grid_height, grid_width, box_height, box_width, 0, 0) == TRUE) {
        print_validation_passed();
        /* Free memory allocation for previous solution */
        return_code_desc.error_code = E_SUCCESS;
        strcpy(return_code_desc.error_message, NO_ERRORS);

    } else {
        /* No solution for the current board */
        return_code_desc.error_code = E_NO_SOLUTION;
        strcpy(return_code_desc.error_message, VALIDATION_FAILED);
    }

    /* Free memory allocation for previous solution */
    free_board(new_solution, grid_height);
    return return_code_desc;

}


/**
 * Count number of possible solutions for the current board
 * @param game_board The game board
 * @param grid_height The height of game board
 * @param grid_width The width of the game board
 * @param box_height The height of a sudoku box
 * @param box_width The width of a sudoku box
 */
returnCodeDesc num_solutions(board game_board, int grid_height, int grid_width, int box_height, int box_width){
    int solutions_count;
    returnCodeDesc return_code_desc;

    if (is_board_errornous(game_board, grid_height, grid_width)) {
        return_code_desc.error_code = E_ERRORNOUS_BOARD;
        strcpy(return_code_desc.error_message, ERROR_BOARD_MSG);
        return return_code_desc;
    }

    solutions_count = count_solutions_iterative(game_board, grid_height, grid_width, box_height, box_width);
    print_num_of_solutions(solutions_count);

    return_code_desc.error_code = E_SUCCESS;
    strcpy(return_code_desc.error_message, NO_ERRORS);
    return return_code_desc;
}

returnCodeDesc autofill(board game_board, int grid_height, int grid_width, int box_height, int box_width) {
    int i;
    int j;
    int num, row, col;
    returnCodeDesc return_code_desc;

    board temp_board = create_empty_board(grid_height, grid_width);
    copy_board(game_board, temp_board, grid_height, grid_width);

    for (i=0; i<grid_height; i++){
        for (j=0; j<grid_width; j++){
            row = i;
            col = j;
            if (is_empty(game_board, row, col) && count_valid_values(game_board, grid_height, grid_width, box_height, box_width,row,col,grid_height) == 1){
                /* Find the one possible valid value for the current cell */
                for (num = 1; num <= (box_height * box_width); num++){
                    if (is_valid(game_board, grid_height, grid_width, box_height, box_width, row, col, num)) {
                        /* Set the cell */
                        temp_board[row][col].value = num;
                        break;
                    }
                }
            }
        }
    }

    copy_board(temp_board, game_board, grid_height, grid_width);
    free_board(temp_board, grid_height);

    return_code_desc.error_code = E_SUCCESS;
    strcpy(return_code_desc.error_message, NO_ERRORS);
    return return_code_desc;

}

returnCodeDesc hint(board game_board, int grid_height, int grid_width, int box_height, int box_width, int row, int col) {
    returnCodeDesc return_code_desc;
    /* Create a temporary board for storing the new solution */
    board new_solution = create_empty_board(grid_height, grid_width);
    copy_board(game_board, new_solution, grid_height, grid_width);

    if (is_board_errornous(game_board, grid_height, grid_width)) {
        return_code_desc.error_code = E_ERRORNOUS_BOARD;
        strcpy(return_code_desc.error_message, ERROR_BOARD_MSG);
    }

    /* Validate the cell is not fixed - if it is print an error and don't perform command */
    else if (game_board[row][col].is_const == TRUE) {
        return_code_desc.error_code = E_CELL_IS_FIXED;
        sprintf(return_code_desc.error_message, CELL_IS_FIXED_ERROR, row, col);
    }

    /* Validate the cell is empty - if it isn't print an error and don't perform command */
    else if (!is_empty(game_board, row, col)){
        return_code_desc.error_code = E_CELL_IS_NOT_EMPTY;
        sprintf(return_code_desc.error_message, CELL_IS_NOT_EMPTY_ERROR, row, col);
    }

    else if (solve_grid(new_solution, grid_height, grid_width, box_height, box_width, 0, 0) == TRUE) {
        print_hint_message(row, col, new_solution[row][col].value);
        return_code_desc.error_code = E_SUCCESS;
        strcpy(return_code_desc.error_message, NO_ERRORS);

    } else {
        /* No solution for the current board */
        return_code_desc.error_code = E_NO_SOLUTION;
        strcpy(return_code_desc.error_message, VALIDATION_FAILED);
    }

    /* Free memory allocation for previous solution */
    free_board(new_solution, grid_height);
    return return_code_desc;

}

returnCodeDesc generate(board game_board, int grid_height, int grid_width, int box_height, int box_width,
        int num_of_cells_to_fill, int num_of_cells_to_keep) {
    returnCodeDesc return_code_desc;
    int i, random_index, row, col, num_of_valid_values;
    int is_error = FALSE;
    int number_of_iterations = 0;
    int filled_cells_count = 0;
    int cleared_cells_count = 0;
    int *valid_values;
    board temp_board = create_empty_board(grid_height, grid_width);

    valid_values = (int*) malloc((box_height * box_width) * sizeof(int));

    if (!valid_values) {
        printf(FUNCTION_FAILED, "malloc");
        exit(EXIT_FAILURE);
    }

    if (count_empty_cells(game_board, grid_height, grid_width) < num_of_cells_to_fill) {
        return_code_desc.error_code = E_INVALID_VALUE;
        strcpy(return_code_desc.error_message, GENERATE_X_ERROR);
        return return_code_desc;
    }

    if (num_of_cells_to_keep > (grid_height * grid_width)){
        return_code_desc.error_code = E_INVALID_VALUE;
        strcpy(return_code_desc.error_message, GENERATE_Y_ERROR);
        return return_code_desc;
    }

    while (number_of_iterations < MAX_ITERATIONS_COUNTER) {
        number_of_iterations++;

        /* Reset the temp board to the original board */
        copy_board(game_board, temp_board, grid_height, grid_width);

        is_error = FALSE;
        filled_cells_count = 0;
        cleared_cells_count = 0;

        while (filled_cells_count < num_of_cells_to_fill) {
            row = (rand() % (grid_height));
            col = (rand() % (grid_width));
            num_of_valid_values = 0;

            if (is_empty(game_board, row, col)) {

                /* Find the possible values for the random cell */
                for (i = 1; i <= (box_height * box_width); i++) {
                    if (is_valid(game_board, grid_height, grid_width, box_height, box_width, row, col, i)) {
                        valid_values[num_of_valid_values] = i;
                        num_of_valid_values++;
                    }
                }

                if (num_of_valid_values > 0) {
                    /* Random cell has got at least one possible valid value */
                    random_index = (rand() % (num_of_valid_values));
                    temp_board[row][col].value = valid_values[random_index];
                    filled_cells_count++;

                }

                else {
                    /* No valid values available to the random empty cell*/
                    is_error = TRUE;
                    break;
                }
            }
        }

        if (is_error == TRUE) {
            /* An error occurred when filling random cells - reset and try again */
            continue;
        }

        /* Finished randomly filling the board. Try to solve it and check if there is a solution*/
        if (solve_grid(temp_board, grid_height, grid_width, box_height, box_height, 0, 0) == FALSE) {
            /* No valid solution to the board - try again */
            continue;
        }

        /* Randomly clear cells from board until only given number are left */
        while (cleared_cells_count < (grid_height * grid_width) - num_of_cells_to_keep) {
            row = (rand() % (grid_height));
            col = (rand() % (grid_width));

            /* Verify cell is not yet empty */
            if (is_empty(game_board, row, col) == FALSE) {
                game_board[row][col].value = 0;
                cleared_cells_count++;
            }
        }

        /* Copy temp board to the original board and free memory */
        copy_board(temp_board, game_board, grid_height, grid_width);

        free(valid_values);
        free_board(temp_board, grid_height);

        return_code_desc.error_code = E_SUCCESS;
        strcpy(return_code_desc.error_message, NO_ERRORS);
        return return_code_desc;
    }

    free(valid_values);
    free_board(temp_board, grid_height);

    return_code_desc.error_code = E_GENERAL_ERROR;
    strcpy(return_code_desc.error_message, "Error in generate method (exceeded 1000 iterations)");
    return return_code_desc;

}


returnCodeDesc solve(board *grid_pointer, char *path, int *grid_height_pointer, int *grid_width_pointer,
                    int *box_height_pointer,
                    int *box_width_pointer) {
    returnCodeDesc return_code_desc;
    int return_code;
    FILE *fd;
    board temp_grid = NULL;

    /* Might need to change to rb */
    fd = fopen(path, "r");

    if (!fd) {
        return_code_desc.error_code = E_OPEN_FILE_FAILED;
        sprintf(return_code_desc.error_message, FAILED_OPENING_FILE, path);
        return return_code_desc;

    }

    /* TODO: Change the return value of read_board_from_file to returnCodeDesc */
    return_code = read_board_from_file(fd, &temp_grid, grid_height_pointer, grid_width_pointer, box_height_pointer,
                                       box_width_pointer);

    if (fclose(fd) != 0) {
        /* Error handling */
        return_code_desc.error_code = E_READ_FROM_FILE_FAILED;
        sprintf(return_code_desc.error_message, FUNCTION_FAILED, "fclose");
        return return_code_desc;
    }

    /* TODO: According to the error we need to know whether to free the temp board with free_board, or just free()
     * or no need to even free it at all */
    if (return_code == FAILURE || temp_grid == NULL) {
        return_code_desc.error_code = E_READ_FROM_FILE_FAILED;
        strcpy(return_code_desc.error_message, READIND_FROM_FILE_ERROR);
        return return_code_desc;
    }

    /* TODO: temp_grid_pointer might not have been initialized - if the return_code_desc from read_board_from_file
     * TODO: is not success - we need to return it */

    /* Check that the loaded board is valid - if not return a proper error */
    if (is_board_valid(temp_grid, *grid_height_pointer, *grid_width_pointer, *box_height_pointer, *box_height_pointer)) {
        return_code_desc.error_code = E_INVALID_BOARD;
        strcpy(return_code_desc.error_message, INVALID_BOARD);
        return return_code_desc;
    }

    if (*grid_pointer != NULL)
        free_board((*grid_pointer), *grid_height_pointer);

    *grid_pointer = temp_grid;
    return_code_desc.error_code = E_SUCCESS;
    strcpy(return_code_desc.error_message, NO_ERRORS);
    return return_code_desc;
}


returnCodeDesc edit(board *grid_pointer, char *path, int *grid_height_pointer, int *grid_width_pointer,
                    int *box_height_pointer,
                    int *box_width_pointer, int path_was_passed) {
    int return_code;
    returnCodeDesc return_code_desc;
    FILE *fd;
    board temp_grid = NULL;

    if (path_was_passed == FALSE){
        *(grid_pointer) = create_empty_board(DEFAULT_GRID_HEIGHT, DEFAULT_GRID_WIDTH);

        *(grid_height_pointer) = DEFAULT_GRID_HEIGHT;
        *(grid_width_pointer) = DEFAULT_GRID_WIDTH;
        *(box_height_pointer) = DEFAULT_BOX_HEIGHT;
        *(box_width_pointer) = DEFAULT_BOX_WIDTH;

        return_code_desc.error_code = E_SUCCESS;
        strcpy(return_code_desc.error_message, NO_ERRORS);
        return return_code_desc;
    } 


    fd = fopen(path, "r");

    if (!fd) {
        return_code_desc.error_code = E_OPEN_FILE_FAILED;
        sprintf(return_code_desc.error_message, FAILED_OPENING_FILE, path);
        return return_code_desc;

    }

    /* TODO: Change the return value of read_board_from_file to returnCodeDesc */
    return_code = read_board_from_file(fd, &temp_grid, grid_height_pointer, grid_width_pointer, box_height_pointer,
                                       box_width_pointer);

    if (fclose(fd) != 0) {
        /* Error handling */
        return_code_desc.error_code = E_READ_FROM_FILE_FAILED;
        sprintf(return_code_desc.error_message, FUNCTION_FAILED, "fclose");
        return return_code_desc;
    }

    /* TODO: According to the error we need to know whether to free the temp board with free_board, or just free() */
    if (return_code == FAILURE || temp_grid == NULL) {
        return_code_desc.error_code = E_READ_FROM_FILE_FAILED;
        strcpy(return_code_desc.error_message, READIND_FROM_FILE_ERROR);
        return return_code_desc;
    }

    /* TODO: temp_grid_pointer might not have been initialized - if the return_code_desc from read_board_from_file
     * TODO: is not success - we need to return it */

    /* In edit mode all the values are non fixed */
    unfix_board(temp_grid, *grid_height_pointer, *grid_width_pointer);

    if (*grid_pointer != NULL)
        free_board((*grid_pointer), *grid_height_pointer);

    *grid_pointer = temp_grid;

    return_code_desc.error_code = E_SUCCESS;
    strcpy(return_code_desc.error_message, NO_ERRORS);
    return return_code_desc;
}

