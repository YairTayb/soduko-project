#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "mainaux.h"

/**
 * Print the current board to the user
 * @param game_board The game board
 * @param grid_height The height of game board
 * @param grid_width The width of the game board
 * @param box_height The height of a sudoku box
 * @param box_width The width of a sudoku box
 */
void print_board(board game_board, int grid_height, int grid_width, int box_height, int box_width,
        game_mode mode, int mark_errors) {
    int i, j, k, line_length, boxes_amount;
    boxes_amount = grid_width / box_width;

    /* The formula for lines given by Moshe. */
    line_length = 4*grid_width + boxes_amount + 1;

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
                printf("|");
            } else if (j % box_width == 0) {
                printf("|");
            }

            if (game_board[i][j].value == UNASSIGNED)
                printf(EMPTY_CELL);

            else if (game_board[i][j].is_const == FALSE) {
                if (game_board[i][j].is_valid == FALSE && (mark_errors == TRUE || mode == edit_mode))
                    printf(ERROR_CELL, game_board[i][j].value);
                else
                    printf(NORMAL_CELL, game_board[i][j].value);
            } else {
                /* Its is a constant value */
                printf(CONST_CELL, game_board[i][j].value);
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
void copy_board(board source_grid, board destination_grid, int grid_height, int grid_width) {
    int i;
    int j;
    for (i = 0; i < grid_height; i++) {
        for (j = 0; j < grid_width; j++) {
            destination_grid[i][j].value = source_grid[i][j].value;
            destination_grid[i][j].is_const = source_grid[i][j].is_const;
            destination_grid[i][j].is_valid = source_grid[i][j].is_valid;
            destination_grid[i][j].has_changed = source_grid[i][j].has_changed;

        }
    }
}

void print_cell_is_not_empty(int row, int col){
    printf(CELL_IS_NOT_EMPTY_ERROR, row, col);
}



/**
 * Create an empty board
 * @param grid_height The board height
 * @param grid_width The board width
 * @return
 */
board create_empty_board(int grid_height, int grid_width) {
    board game_board = (board) malloc(grid_height * sizeof(struct Cell*));/**/
    int i;
    int j;

    /*check if malloc failed*/
    if(!game_board){
        printf(FUNCTION_FAILED, "malloc");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < grid_height; i++){
        game_board[i] = (struct Cell *) malloc(grid_width * sizeof(struct Cell));
        /*check if malloc failed*/
        if(!game_board[i]){
            printf(FUNCTION_FAILED, "malloc");
            exit(EXIT_FAILURE);
        }
        
    }
        
    /* Initiate the board */
    for (i = 0; i < grid_height; i++) {
        for (j = 0; j < grid_width; j++) {
            game_board[i][j].value = UNASSIGNED;
            game_board[i][j].is_const = FALSE;
            game_board[i][j].has_changed = FALSE;
        }
    }

    return game_board;
}

/**
 * Empty a given board
 * @param board_to_empty The board to empty
 * @param grid_height The board height
 * @param grid_width The board width
 */
void empty_board(board board_to_empty, int grid_height, int grid_width) {
    int i, j;
    for (i = 0; i < grid_height; i++) {
        for (j = 0; j < grid_width; j++) {
            board_to_empty[i][j].value = UNASSIGNED;
            board_to_empty[i][j].is_const = FALSE;
        }
    }
}

void free_board(board game_board, int grid_height){
    int i;

    for ( i=0 ; i < grid_height; i++ ){
        free(game_board[i]);
    }

    free(game_board);
}

int is_board_errornous(board board, int grid_height, int grid_width) {
    int i, j;
    for (i = 0; i < grid_height; i++) {
        for (j = 0; j < grid_width; j++) {
            if (board[i][j].is_valid == FALSE)
                return TRUE;
        }
    }

    return FALSE;
}

void unfix_board(board board, int grid_height, int grid_width) {
    int i, j;
    for (i = 0; i < grid_height; i++) {
        for (j = 0; j < grid_width; j++) {
            board[i][j].is_const = FALSE;
        }
    }
}

/**
 * Check if an input is in valid range
 * @param num The input
 * @param max_num_in_range The upper bound of the range
 * @return 1 = Valid, 0 = Invalid.
 */
int is_valid_input(int num, int max_num_in_range) {
    return num >= 0 && num <= max_num_in_range;
}

/**
 * Print validation failed message.
 */
void print_validation_failed() {
    printf(VALIDATION_FAILED);
}

/**
 * Print validation passed message.
 */
void print_validation_passed() {
    printf(VALIDATION_PASSED);
}

/**
 * Print invalid value error
 */
void print_invalid_value(int lower_limit, int upper_limit) {
    printf(INVALID_VALUE_ERROR, lower_limit, upper_limit);
}

/**
 * Print invalid value error
 */
void print_num_of_solutions(int solutions_count) {
    printf(SOLUTIONS_COUNT_MSG, solutions_count);
}

/**
 * Print fixed cell error
 */
void print_fixed_cell_error(int row, int col) {
    printf(CELL_IS_FIXED_ERROR, row, col);
}

/**
 * Print winning message
 */
void print_winning_message() {
    printf(WIN_MSG);
}

/**
 * Print errornous board message
 */
void print_errornous_board_message() {
    printf(ERROR_BOARD_MSG);
}

/**
 * Print hint message
 * @param hint_value The hint value to print
 */
void print_hint_message(int row, int col, int hint_value) {
    printf(HINT_MSG, row, col, hint_value);
}

void handle_errors(returnCodeDesc return_code_desc){
    if (return_code_desc.error_code != E_SUCCESS) {
        /* An error occurred while parsing command */
        if (return_code_desc.error_code != E_BLANK_LINE)
            printf("%s", return_code_desc.error_message);

        if (return_code_desc.error_code == E_FUNCTION_FAILED) {
            /* One of the functions have failed - we may exit without cleaning */
            printf(EXIT_MSG);
            exit(EXIT_FAILURE);
        }
    }
}

int is_error(returnCodeDesc return_code_desc){
    if (return_code_desc.error_code != E_SUCCESS)
        return TRUE;
    return FALSE;
}

int is_empty_string(char *s) {
    while (*s != '\0') {
        if (!isspace((unsigned char)*s))
            return FALSE;
        s++;
    }
    return TRUE;
}


double get_random_in_range(double min, double max) {
    return min + (rand() / (RAND_MAX / (max - min)));
}