#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "gurobi_solver.h"
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
    if (!is_valid_input(row, grid_height)) {
        return_code_desc.error_code = E_INVALID_VALUE;
        sprintf(return_code_desc.error_message, INVALID_VALUE_ERROR, 1, grid_height);
        return return_code_desc;
    }

    if (!is_valid_input(col, grid_width)) {
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
        sprintf(return_code_desc.error_message, CELL_IS_FIXED_ERROR, row + 1, col + 1);
        return return_code_desc;
    }

    /* Set the cell */
    game_board[row][col].value = value;
    game_board[row][col].has_changed = TRUE;

    return_code_desc.error_code = E_SUCCESS;
    strcpy(return_code_desc.error_message, NO_ERRORS);
    return return_code_desc;

}

void print_changes(board before, board after, int grid_height, int grid_width, command_changed_from comm){

    int i, j;
    for(i = 0;i < grid_height; i++){
        for(j = 0; j < grid_width; j++){
            /*check if the value has changed*/
            if (after[i][j].has_changed == TRUE) {
                switch (comm) {
                    case redo:
                        printf("Redo ");
                        printf("(%d,%d) from %d to %d.\n", (i + 1), (j + 1), before[i][j].value, after[i][j].value);
                        break;
                    case undo:
                        printf("Undo ");
                        printf("(%d,%d) from %d to %d.\n", (i + 1), (j + 1), after[i][j].value, before[i][j].value);
                        break;
                    default:
                        break;
                }
            }
        }
    }
}


returnCodeDesc undo_move(board game_board, int grid_height, int grid_width, struct MovesList *list) {

    struct Node* current_board = list -> current_move ;
    struct Node* before_board = psuedo_undo(list);
    returnCodeDesc return_value;

    if (before_board == NULL) {
        strcpy(return_value.error_message, CANNOT_UNDO);
        return_value.error_code = E_CANNOT_UNDO;
        return return_value;
    }

    else if (current_board == NULL) {
            strcpy(return_value.error_message, CANNOT_UNDO);
            return_value.error_code = E_CANNOT_UNDO;
            return return_value;

    }

    else {
        print_changes(before_board->data, current_board->data, grid_height,grid_width,undo);
        copy_board(before_board->data, game_board, grid_height, grid_width);
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
        print_changes(game_board, temp->data,grid_height,grid_width,redo);
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
        if (is_board_errornous(game_board, grid_height, grid_width) == TRUE) {
            free_board(board_copy, grid_height);
            return_code_desc.error_code = E_ERRORNOUS_BOARD;
            strcpy(return_code_desc.error_message, ERROR_BOARD_MSG);
            return return_code_desc;
        }

        return_code_desc = solve_gurobi(board_copy, grid_height, grid_width, box_height, box_width, ILP, NULL);

        /* Check if there were errors while running the LP solver.
         * If An error has occurred while running the Gurobi LP solver, then whether the board is unsolvable,
         * or there was an error in the runtime of the solver - raise the error. */
        if (is_error(return_code_desc) == TRUE) {
            free_board(board_copy, grid_height);
            return return_code_desc;
        }
    }

    fd = fopen(path, "wb");

    if (!fd) {
        free_board(board_copy, grid_height);
        return_code_desc.error_code = E_OPEN_FILE_FAILED;
        sprintf(return_code_desc.error_message, FAILED_OPENING_FILE, strerror(errno), path);
        return return_code_desc;

    } else {
        return_code_desc = write_board_to_file(game_board, grid_height, grid_width, box_width, box_height, fd, mode);
    }

    if (fclose(fd) != 0) {
        /* Error handling */
        free_board(board_copy, grid_height);
        return_code_desc.error_code = E_READ_FROM_FILE_FAILED;
        sprintf(return_code_desc.error_message, FUNCTION_FAILED, "fclose");
        return return_code_desc;
    }

    free_board(board_copy, grid_height);
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

    if (is_board_errornous(game_board, grid_height, grid_width) == TRUE) {
        return_code_desc.error_code = E_ERRORNOUS_BOARD;
        strcpy(return_code_desc.error_message, ERROR_BOARD_MSG);
    }

    else {
        return_code_desc = solve_gurobi(new_solution, grid_height, grid_width, box_height, box_width, ILP, NULL);

        /* Check if there were errors while running the ILP solver.
         * If An error has occurred while running the Gurobi ILP solver, then whether the board is unsolvable,
         * or there was an error in the runtime of the solver - raise the error.
         * Otherwise the validation has passed. */
        if (is_error(return_code_desc) == FALSE) {
            print_validation_passed();
            return_code_desc.error_code = E_SUCCESS;
            strcpy(return_code_desc.error_message, NO_ERRORS);
        }

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

    if (is_board_errornous(game_board, grid_height, grid_width) == TRUE) {
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
    int num, row, col;
    returnCodeDesc return_code_desc;

    board temp_board = create_empty_board(grid_height, grid_width);
    copy_board(game_board, temp_board, grid_height, grid_width);

    if (is_board_errornous(game_board, grid_height, grid_width) == TRUE) {
        return_code_desc.error_code = E_ERRORNOUS_BOARD;
        strcpy(return_code_desc.error_message, ERROR_BOARD_MSG);
        free_board(temp_board, grid_height);
        return return_code_desc;
    }

    for (row=0; row<grid_height; row++){
        for (col=0; col<grid_width; col++){
            if (is_empty(game_board, row, col) && count_valid_values(game_board, grid_height, grid_width, box_height, box_width,row,col,grid_height) == 1){
                /* Find the one possible valid value for the current cell */
                for (num = 1; num <= (box_height * box_width); num++){
                    if (is_valid(game_board, grid_height, grid_width, box_height, box_width, row, col, num)) {
                        /* Set the cell */
                        printf("Cell (%d,%d) from %d to %d. \n",col,row,temp_board[row][col].value,num);
                        temp_board[row][col].value = num;
                        temp_board[row][col].has_changed = TRUE;
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

    /* Validate the input */
    if (!is_valid_input(row, grid_height)) {
        return_code_desc.error_code = E_INVALID_VALUE;
        sprintf(return_code_desc.error_message, INVALID_VALUE_ERROR, 1, grid_height);
    }

    else if (!is_valid_input(col, grid_width)) {
        return_code_desc.error_code = E_INVALID_VALUE;
        sprintf(return_code_desc.error_message, INVALID_VALUE_ERROR, 1, grid_height);
    }

    else if (is_board_errornous(game_board, grid_height, grid_width) == TRUE) {
        return_code_desc.error_code = E_ERRORNOUS_BOARD;
        strcpy(return_code_desc.error_message, ERROR_BOARD_MSG);
    }

    /* Validate the cell is not fixed - if it is print an error and don't perform command */
    else if (game_board[row][col].is_const == TRUE) {
        return_code_desc.error_code = E_CELL_IS_FIXED;
        sprintf(return_code_desc.error_message, CELL_IS_FIXED_ERROR, row + 1, col + 1);
    }

    /* Validate the cell is empty - if it isn't print an error and don't perform command */
    else if (!is_empty(game_board, row, col)){
        return_code_desc.error_code = E_CELL_IS_NOT_EMPTY;
        sprintf(return_code_desc.error_message, CELL_IS_NOT_EMPTY_ERROR, row + 1, col + 1);
    }
    
    else 
    {
        return_code_desc = solve_gurobi(new_solution, grid_height, grid_width, box_height, box_width, ILP, NULL);

        /* Check if there were errors while running the LP solver.
         * If An error has occurred while running the Gurobi LP solver, then whether the board is unsolvable,
         * or there was an error in the runtime of the solver - raise the error. */
        if (is_error(return_code_desc) == FALSE) {
            printf(HINT_MSG, row + 1, col + 1, new_solution[row][col].value);
            return_code_desc.error_code = E_SUCCESS;
            strcpy(return_code_desc.error_message, NO_ERRORS);
        }
        else {
            /* No solution for the current board */
            return_code_desc.error_code = E_NO_SOLUTION;
            strcpy(return_code_desc.error_message, VALIDATION_FAILED);
        }
    }

    /* Free memory allocation for previous solution */
    free_board(new_solution, grid_height);
    return return_code_desc;

}

returnCodeDesc generate(board game_board, int grid_height, int grid_width, int box_height, int box_width,
        int num_of_cells_to_fill, int num_of_cells_to_keep) {
    returnCodeDesc return_code_desc;
    int i, random_index, row, col, num_of_valid_values;
    int error_occurred = FALSE;
    int number_of_iterations = 0;
    int filled_cells_count = 0;
    int cleared_cells_count = 0;
    int *valid_values;
    board temp_board = create_empty_board(grid_height, grid_width);

    /*Should not run if the board is erronous - wasting cpu time*/
    if(is_board_errornous(game_board, grid_height, grid_width)){
        return_code_desc.error_code = E_GENERAL_ERROR;
        strcpy(return_code_desc.error_message,BOARD_IS_ERRONOUS);
        return return_code_desc;
    }

    valid_values = (int*) malloc((box_height * box_width) * sizeof(int));

    if (valid_values == NULL) {
        return_code_desc.error_code = E_FUNCTION_FAILED;
        sprintf(return_code_desc.error_message, FUNCTION_FAILED, "malloc");
        return return_code_desc;
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

        error_occurred = FALSE;
        filled_cells_count = 0;
        cleared_cells_count = 0;

        while (filled_cells_count < num_of_cells_to_fill) {
            row = (rand() % (grid_height));
            col = (rand() % (grid_width));
            num_of_valid_values = 0;

            if (is_empty(temp_board, row, col) == TRUE) {

                /* Find the possible values for the random cell */
                for (i = 1; i <= (box_height * box_width); i++) {
                    if (is_valid(temp_board, grid_height, grid_width, box_height, box_width, row, col, i)) {
                        valid_values[num_of_valid_values] = i;
                        num_of_valid_values++;
                    }
                }

                if (num_of_valid_values > 0) {
                    /* Random cell has got at least one possible valid value */
                    random_index = (rand() % (num_of_valid_values));
                    temp_board[row][col].value = valid_values[random_index];
                    temp_board[row][col].has_changed = TRUE;
                    filled_cells_count++;

                }

                else {
                    /* No valid values available to the random empty cell*/
                    error_occurred = TRUE;
                    break;
                }
            }
        }

        if (error_occurred == TRUE) {
            /* An error occurred when filling random cells - reset and try again */
            continue;
        }

        /* Finished randomly filling the board. Try to solve it and check if there is a solution*/
        return_code_desc = solve_gurobi(temp_board, grid_height, grid_width, box_height, box_width, ILP, NULL);

        /* Check if there were errors while running the LP solver.
         * If An error has occurred while running the Gurobi LP solver, then whether the board is unsolvable,
         * or there was an error in the runtime of the solver - raise the error. */
        if (is_error(return_code_desc) == TRUE) {
            continue;
        }
        if (solve_grid(temp_board, grid_height, grid_width, box_height, box_width, 0, 0) == FALSE) {
            /* No valid solution to the board - try again */
            continue;
        }

        /* Randomly clear cells from board until only given number are left */
        while ((cleared_cells_count + num_of_cells_to_keep) < (grid_height * grid_width)) {
            row = (rand() % (grid_height));
            col = (rand() % (grid_width));

            /* Verify cell is not yet empty */
            if (is_empty(temp_board, row, col) == FALSE) {
                temp_board[row][col].value = UNASSIGNED;
                temp_board[row][col].has_changed = TRUE;
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
    sprintf(return_code_desc.error_message, GENERATE_EXCEEDED_ITERATIONS, MAX_ITERATIONS_COUNTER);
    return return_code_desc;

}


returnCodeDesc solve(board *grid_pointer, char *path, int *grid_height_pointer, int *grid_width_pointer,
                    int *box_height_pointer,
                    int *box_width_pointer) {

    returnCodeDesc return_code_desc;
    FILE *fd;
    board temp_grid = NULL;

    /* Might need to change to rb */
    fd = fopen(path, "rb");

    if (!fd) {
        perror("Error: ");
        return_code_desc.error_code = E_OPEN_FILE_FAILED;
        sprintf(return_code_desc.error_message, FAILED_OPENING_FILE, strerror(errno), path);
        return return_code_desc;

    }

    return_code_desc = read_board_from_file(fd, &temp_grid, grid_height_pointer, grid_width_pointer, box_height_pointer,
                                       box_width_pointer);

    if (fclose(fd) != 0) {
        return_code_desc.error_code = E_READ_FROM_FILE_FAILED;
        sprintf(return_code_desc.error_message, FUNCTION_FAILED, "fclose");
        return return_code_desc;
    }

    if (is_error(return_code_desc) == TRUE) {
        return return_code_desc;
    }

    /* Check that the loaded board is valid - if not return a proper error */
    if (is_board_valid(temp_grid, *grid_height_pointer, *grid_width_pointer, *box_height_pointer, *box_height_pointer) == FALSE) {
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

    fd = fopen(path, "rb");

    if (!fd) {
        return_code_desc.error_code = E_OPEN_FILE_FAILED;
        sprintf(return_code_desc.error_message, FAILED_OPENING_FILE, strerror(errno), path);
        return return_code_desc;
    }

    return_code_desc = read_board_from_file(fd, &temp_grid, grid_height_pointer, grid_width_pointer, box_height_pointer,
                                       box_width_pointer);

    if (fclose(fd) != 0) {
        /* Error handling */
        return_code_desc.error_code = E_READ_FROM_FILE_FAILED;
        sprintf(return_code_desc.error_message, FUNCTION_FAILED, "fclose");
        return return_code_desc;
    }

    if (is_error(return_code_desc) == TRUE) {
        return return_code_desc;
    }

    /* In edit mode all the values are non fixed */
    unfix_board(temp_grid, *grid_height_pointer, *grid_width_pointer);

    if (*grid_pointer != NULL)
        free_board((*grid_pointer), *grid_height_pointer);

    *grid_pointer = temp_grid;

    return_code_desc.error_code = E_SUCCESS;
    strcpy(return_code_desc.error_message, NO_ERRORS);
    return return_code_desc;
}

returnCodeDesc set_mark_errors(int* mark_errors, int input){
    returnCodeDesc return_code_desc;

    if (input != 0 && input != 1) {
        return_code_desc.error_code = E_INVALID_VALUE;
        strcpy(return_code_desc.error_message, INVALID_MARK_ERRORS_VALUE);
        return return_code_desc;
    }

    *mark_errors = input;
    return_code_desc.error_code = E_SUCCESS;
    strcpy(return_code_desc.error_message, NO_ERRORS);
    return return_code_desc;
}


returnCodeDesc guess(board game_board, int grid_height, int grid_width, int box_height, int box_width, double threshold) {
    returnCodeDesc return_code_desc;
    int row, col;
    double scores_sum = 0.0;
    double normalised_score = 0.0;
    double random_value = 0;
    double min = 1.0;
    int value;
    int chosen_value = -1;

    double *guess_scores = NULL;

    /* Validate the input */
    if (threshold <= 0.0 || threshold >= 1.0) {
        return_code_desc.error_code = E_INVALID_VALUE;
        sprintf(return_code_desc.error_message, INVALID_DOUBLE_VALUE_ERROR, 0.0, 1.0);
    }

    else if (is_board_errornous(game_board, grid_height, grid_width) == TRUE) {
        return_code_desc.error_code = E_ERRORNOUS_BOARD;
        strcpy(return_code_desc.error_message, ERROR_BOARD_MSG);
    }
    else {
        guess_scores = (double *) malloc(grid_height * grid_width * (box_height * box_width) * sizeof(double));

        if (guess_scores == NULL) {
            return_code_desc.error_code = E_FUNCTION_FAILED;
            sprintf(return_code_desc.error_message, FUNCTION_FAILED, "malloc");
            return return_code_desc;
        }

        return_code_desc = solve_gurobi(game_board, grid_height, grid_width, box_height, box_width, LP, guess_scores);

        /* Check if there were errors while running the LP solver.
         * If An error has occurred while running the Gurobi LP solver, then whether the board is unsolvable,
         * or there was an error in the runtime of the solver - raise the error. */
        if (is_error(return_code_desc) == TRUE) {
            free(guess_scores);
            return return_code_desc;
        }

        for(row = 0; row < grid_height; row++){
            for (col = 0; col < grid_width; col++) {
                if (is_empty(game_board, row, col)) {
                    scores_sum = 0.0;
                    normalised_score = 0.0;
                    chosen_value = -1;
                    min = 1.0;

                    for (value = 0; value < (box_height * box_width); value++) {
                        if (is_valid(game_board, grid_height, grid_width, box_height, box_width, row, col, value + 1) &&
                            (guess_scores[MULTIDIM_ARR_LOC(row, col, value, grid_height, grid_width,
                                                           (box_height * box_width))] >= threshold)) {
                            scores_sum += guess_scores[MULTIDIM_ARR_LOC(row, col, value, grid_height, grid_width,
                                                                        (box_height * box_width))];
                        }
                        else {
                            guess_scores[MULTIDIM_ARR_LOC(row, col, value, grid_height, grid_width,
                                                          (box_height * box_width))] = 0;
                        }
                    }

                    if (scores_sum > 0.0) {
                        for (value = 0; value < (box_height * box_width); value++) {
                            if (guess_scores[MULTIDIM_ARR_LOC(row, col, value, grid_height, grid_width,
                                                              (box_height * box_width))] > 0) {
                                normalised_score += (guess_scores[MULTIDIM_ARR_LOC(row, col, value, grid_height, grid_width,
                                                                                   (box_height * box_width))]) / scores_sum;
                                guess_scores[MULTIDIM_ARR_LOC(row, col, value, grid_height, grid_width,
                                                              (box_height * box_width))] = normalised_score;
                            }
                        }

                        random_value = get_random_in_range(0.0, 1.0);

                        for (value = 0; value < (box_height * box_width); value++) {
                            if (guess_scores[MULTIDIM_ARR_LOC(row, col, value, grid_height, grid_width,
                                                              (box_height * box_width))] >= random_value &&
                                    guess_scores[MULTIDIM_ARR_LOC(row, col, value, grid_height, grid_width,
                                                                  (box_height * box_width))]  <= min) {
                                    min = guess_scores[MULTIDIM_ARR_LOC(row, col, value, grid_height, grid_width,
                                                                        (box_height * box_width))];
                                    chosen_value = value + 1;
                            }
                        }
                    }

                    if (chosen_value != -1) {
                        game_board[row][col].value = chosen_value;
                        game_board[row][col].has_changed = TRUE;
                    }
                }
            }
        }

        free(guess_scores);
        return_code_desc.error_code = E_SUCCESS;
        strcpy(return_code_desc.error_message, NO_ERRORS);
    }

    return return_code_desc;

}

returnCodeDesc
guess_hint(board game_board, int grid_height, int grid_width, int box_height, int box_width, int row, int col) {
    returnCodeDesc return_code_desc;
    int value;
    double *guess_scores = NULL;

    /* Validate the input */
    if (!is_valid_input(row, grid_height)) {
        return_code_desc.error_code = E_INVALID_VALUE;
        sprintf(return_code_desc.error_message, INVALID_VALUE_ERROR, 1, grid_height);
    }

    else if (!is_valid_input(col, grid_width)) {
        return_code_desc.error_code = E_INVALID_VALUE;
        sprintf(return_code_desc.error_message, INVALID_VALUE_ERROR, 1, grid_height);
    }

    else if (is_board_errornous(game_board, grid_height, grid_width) == TRUE) {
        return_code_desc.error_code = E_ERRORNOUS_BOARD;
        strcpy(return_code_desc.error_message, ERROR_BOARD_MSG);
    }

    /* Validate the cell is not fixed - if it is print an error and don't perform command */
    else if (game_board[row][col].is_const == TRUE) {
        return_code_desc.error_code = E_CELL_IS_FIXED;
        sprintf(return_code_desc.error_message, CELL_IS_FIXED_ERROR, row + 1, col + 1);
    }

    /* Validate the cell is empty - if it isn't print an error and don't perform command */
    else if (!is_empty(game_board, row, col)){
        return_code_desc.error_code = E_CELL_IS_NOT_EMPTY;
        sprintf(return_code_desc.error_message, CELL_IS_NOT_EMPTY_ERROR, row + 1, col + 1);
    }

    else {
        guess_scores = (double *) malloc(grid_height * grid_width * (box_height * box_width) * sizeof(double));

        if (guess_scores == NULL) {
            return_code_desc.error_code = E_FUNCTION_FAILED;
            sprintf(return_code_desc.error_message, FUNCTION_FAILED, "malloc");
            return return_code_desc;
        }

        return_code_desc = solve_gurobi(game_board, grid_height, grid_width, box_height, box_width, LP, guess_scores);

        /* Check if there were errors while running the LP solver.
         * If An error has occurred while running the Gurobi LP solver, then whether the board is unsolvable,
         * or there was an error in the runtime of the solver - raise the error. */
        if (is_error(return_code_desc) == TRUE) {
            free(guess_scores);
            return return_code_desc;
        }

        for (value = 0; value < (box_height * box_width); value++) {
            if (is_valid(game_board, grid_height, grid_width, box_height, box_width, row, col, value + 1) &&
                (guess_scores[MULTIDIM_ARR_LOC(row, col, value, grid_height, grid_width,
                                               (box_height * box_width))] > 0.0)) {
                printf(GUESS_HINT_MSG, row + 1, col + 1, value + 1, guess_scores[MULTIDIM_ARR_LOC(row, col, value, grid_height, grid_width,
                                                                                          (box_height * box_width))]);
            }
        }

        return_code_desc.error_code = E_SUCCESS;
        strcpy(return_code_desc.error_message, NO_ERRORS);
        free(guess_scores);
    }

    return return_code_desc;

}