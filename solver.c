#include "solver.h"
#include "stack.h"
#include "mainaux.h"

/**
 * Count the number of empty cells in the board
 * @param board The game board
 * @param grid_height The board height
 * @param grid_width The board width
 * @param box_height The box height
 * @param box_width The box width
 * @return The number of empty cells in the board
 */
int count_empty_cells(board game_board, int grid_height, int grid_width){
    int i, j;
    int counter = 0;
    for (i = 0; i < grid_height; i++) {
        for (j = 0; j < grid_width; j++) {
            /* Check if the call is valid and mark properly */
            if (is_empty(game_board, i, j)) {
                counter++;
            }
        }
    }

    return counter;
}

/**
 * Update the errors on the board. Iterate over every cell in the board and check if its value is valid.
 * If it is valid, mark the cell as valid, otherwise mark as error.
 * @param board The game board
 * @param grid_height The board height
 * @param grid_width The board width
 * @param box_height The box height
 * @param box_width The box width
 */
void update_board_errors(board game_board, int grid_height, int grid_width, int box_height, int box_width){
    int row, col;
    for (row = 0; row < grid_height; row++) {
        for (col = 0; col < grid_width; col++) {
            /* A fixed cell can never be errornous */
            if (game_board[row][col].is_const == FALSE) {
                if (is_valid(game_board, grid_height, grid_width, box_height, box_width, row, col,
                             game_board[row][col].value))
                    game_board[row][col].is_valid = TRUE;
                else
                    game_board[row][col].is_valid = FALSE;
            }
        }
    }
}

/**
 * Validate that a given board is valid. A valid board is a board containing no conflicts between fixed cells.
 * @param board The game board
 * @param grid_height The board height
 * @param grid_width The board width
 * @param box_height The box height
 * @param box_width The box width
 * @return 1 = the board is valid, 0 = board is invalid
 */
int is_board_valid(board game_board, int grid_height, int grid_width, int box_height, int box_width){
    int row, col;
    int is_valid;
    board temp_board = create_empty_board(grid_height, grid_width);
    copy_board(game_board, temp_board, grid_height, grid_width);

    /* Check that the game doesn't contain any empty fixed cells */
    for (row = 0; row < grid_height; row++) {
        for (col = 0; col < grid_width; col++) {
            if (game_board[row][col].is_const == TRUE && game_board[row][col].value == UNASSIGNED) {
                free_board(temp_board, grid_height);
                return FALSE;
            }
        }
    }

    /* Clear the temp board of all non-fixed cells and set all fixed cells to non-fixed and mark them as valid */
    for (row = 0; row < grid_height; row++) {
        for (col = 0; col < grid_width; col++) {
            if (game_board[row][col].is_const == FALSE) {
                /* Clear the non-fixed cell */
                temp_board[row][col].value = UNASSIGNED;
            }

            temp_board[row][col].is_const = FALSE;
            temp_board[row][col].is_valid = TRUE;
        }
    }

    /* Check the temp board for errors (errors in the temp board means that there are conflicting fixed cells in the
     * original board */
    update_board_errors(temp_board, grid_height, grid_width, box_height, box_width);
    if (is_board_errornous(temp_board, grid_height, grid_width) == TRUE) {
        is_valid = FALSE;
    }

    else {
        is_valid = TRUE;
    }

    free_board(temp_board, grid_height);
    return is_valid;

}


/**
 * Check whether a number is already used in a given row
 * @param game_board The game board
 * @param grid_width The width of the board
 * @param row The row to search the number for
 * @param current_col The current col of the cell we want to place the number at
 * @param num The number to place
 * @return 1 = Not used, 0 = Used in row.
 */
int used_in_row(board game_board, int grid_width, int row, int current_col, int num) {
    int col;
    for (col = 0; col < grid_width; col++)
        if (current_col != col && game_board[row][col].value == num)
            return TRUE;
    return FALSE;
}

/**
 * Check whether a number is already used in a given col
 * @param game_board The game board
 * @param grid_height The height of the board
 * @param current_row The current row of the cell we want to place the number at
 * @param col The col to search the number for
 * @param num The number to place
 * @return 1 = Not used, 0 = Used in col.
 */
int used_in_col(board game_board, int grid_height, int current_row, int col, int num) {
    int row;
    for (row = 0; row < grid_height; row++)
        if (current_row != row && game_board[row][col].value == num)
            return TRUE;
    return FALSE;
}

/**
 * Check whether a given number is already used in a given box
 * @param game_board The game board
 * @param box_start_row The box's starting row
 * @param box_start_col The box's starting col
 * @param box_height The box height
 * @param box_width The box width
 * @param current_row The current row to place the num at
 * @param current_col The current col to place the num at
 * @param num The number to place
 * @return 1 = No used, 0 = Used in the box.
 */
int
used_in_box(board game_board, int box_start_row, int box_start_col, int box_height, int box_width, int current_row,
            int current_col, int num) {
    int row;
    int col;
    for (row = box_start_row; row < (box_start_row + box_height); row++) {
        for (col = box_start_col; col < (box_start_col + box_width); col++) {
            if (!(current_row == row && current_col == col) &&
                    game_board[row][col].value == num)
                return TRUE;
        }
    }
    return FALSE;
}

/**
 * Check whether it is valid to place a number in a given location
 * @param game_board The game board
 * @param grid_height The board height
 * @param grid_width The board width
 * @param box_height The box height
 * @param box_width The box width
 * @param row The row to place the number at
 * @param col The column to place the number at
 * @param num The number to place
 * @return 1 = Valid, 0 = Invalid.
 */
int is_valid(board game_board, int grid_height, int grid_width, int box_height, int box_width, int row, int col,
             int num) {
    if (num != UNASSIGNED)
        return !used_in_row(game_board, grid_width, row, col, num) &&
               !used_in_col(game_board, grid_height, row, col, num) &&
               !used_in_box(game_board, row - row % box_height, col - col % box_width, box_height, box_width, row, col, num);
    return TRUE;
}

/**
 * Count valid values for a given cell
 * @param game_board The game board
 * @param grid_height The board height
 * @param grid_width The board width
 * @param box_height the box height
 * @param box_width The box width
 * @param row The row of the cell
 * @param col The column of the cell
 * @param values The values array to fill with valid values
 * @param range Upper bound of the numbers range that is valid for the cell.
 * @return Number of valid values found
 */
int count_valid_values(board game_board, int grid_height, int grid_width, int box_height, int box_width, int row, int col,
                  int range) {
    int num;
    int counter = 0;
    for (num = 1; num <= range; num++) {
        /* For each number up to the range bound - check if its valid */
        if (is_valid(game_board, grid_height, grid_width, box_height, box_width, row, col, num)) {
            counter++;
        }
    }

    return counter;
}

/**
 * Find valid values for a given cell
 * @param game_board The game board
 * @param grid_height The board height
 * @param grid_width The board width
 * @param box_height the box height
 * @param box_width The box width
 * @param row The row of the cell
 * @param col The column of the cell
 * @param values The values array to fill with valid values
 * @param range Upper bound of the numbers range that is valid for the cell.
 * @return Number of valid values found
 */
int find_valid_values(int* values, board game_board, int grid_height, int grid_width, int box_height, int box_width, int row, int col,
                       int range) {
    int num;
    int counter = 0;
    int i = 0;
    for (num = 1; num <= range; num++) {
        /* For each number up to the range bound - check if its valid */
        if (is_valid(game_board, grid_height, grid_width, box_height, box_width, row, col, num)) {
            values[i] = num;
            i++;
            counter++;
        }
    }

    return counter;
}

/**
 * Check whether the cell is empty
 * @param game_board The game board
 * @param row The row of the cell
 * @param col The column of the cell
 * @return 1 = Empty, 0 = Filled
 */
int is_empty(board game_board, int row, int col) {
    if (game_board[row][col].value == UNASSIGNED)
        return TRUE;
    return FALSE;
}

/**
 * Check whether the cell is valid or errornous
 * @param game_board The game board
 * @param row The row of the cell
 * @param col The column of the cell
 * @return 1 = errornous, 0 = valid
 */
int is_cell_errornous(board game_board, int row, int col) {
    if (game_board[row][col].is_valid == FALSE)
        return TRUE;
    return FALSE;
}


/**
 * Count the number of solutions of a given board
 * @param game_board The game board
 * @param grid_height The board height
 * @param grid_width The width of the board
 * @param box_height The box height
 * @param box_width The box width
 * @param row The row of the cell to start solving from
 * @param col The column of the cell to start solving from
 * @return 1 = Solved, 0 = No solution found
 */
int count_solutions(board game_board, int grid_height, int grid_width, int box_height, int box_width, int row, int col) {
    int count = 0;
    int i;

    if (row >= grid_height) {
        /* Went through the whole board and didnt solve the sudoku. */
        return 1;
    }
    /* Boundaries check */
    if (col >= grid_width) {
        return count_solutions(game_board, grid_height, grid_width, box_height, box_width, row + 1, 0);
    }

    /* Correct solution of a board */

    if (game_board[row][col].is_const == TRUE || game_board[row][col].value != UNASSIGNED) {
        /* We are within a legitimate cell. */
        return count_solutions(game_board, grid_height, grid_width, box_height, box_width, row, col + 1);
    }

    /*maybe width?*/
    for (i = 1; i <= (box_height * box_width); i++) {
        if (is_valid(game_board, grid_height, grid_width, box_height, box_width, row, col, i)) {

            game_board[row][col].value = i;
            count += count_solutions(game_board, grid_height, grid_width, box_height, box_width, row, col + 1);
            game_board[row][col].value = UNASSIGNED;

        }
    }

    return count;
}

int _count_solutions_iterative(board game_board, int grid_height, int grid_width, int box_height, int box_width,
        int row, int col, stack* s){
    int count = 0;
    move m;
    int j;
    int found = FALSE;
    int val = 1;

    while (!empty(s)) {

        if (row >= grid_height) {
            count++;
            m = pop(s);
            row = m.row;
            col = m.col;
            val = m.value + 1;
            game_board[row][col].value = UNASSIGNED;
            continue;
        }

        if (game_board[row][col].is_const == TRUE || game_board[row][col].value != UNASSIGNED) {
            /* We are within a legitimate cell. */
            if (col + 1 >= grid_width) {
                row++;
                col = 0;
            }

            else {
                col++;
            }

            continue;
        }

        found = FALSE;
        for (j = val; j <= (box_height * box_width); j++) {
            if (is_valid(game_board, grid_height, grid_width, box_height, box_width, row, col, j)) {
                game_board[row][col].value = j;
                m.row = row;
                m.col = col;
                m.value = j;
                push(s, m);
                found = TRUE;
                val = 1;

                if (col + 1>= grid_width) {
                    row++;
                    col = 0;
                }

                else {
                    col++;
                }

                break;
            }
        }
        if (found == FALSE){
            game_board[row][col].value = UNASSIGNED;
            m = pop(s);
            row =m.row;
            col = m.col;
            val = m.value + 1;
            game_board[row][col].value = UNASSIGNED;
        }
    }

    return count;
}

int count_solutions_iterative(board game_board, int grid_height, int grid_width, int box_height, int box_width) {
    stack s;
    move m;
    int row = 0;
    int col = 0;
    int count = 0;
    int i;

    initialize(&s);

    while (TRUE){

        /* Reached on of the board */
        if (row >= grid_height){
            /* No empty cells - only one solutions exists */
            return 1;
        }

        if (is_empty(game_board, row, col)) {
            break;
        }

        else if (col + 1>= grid_width) {
            row++;
            col = 0;
        }

        else {
            col++;
        }
    }

    /* Find a valid value for the first empty cell*/
    for (i=1; i<=(box_height*box_width); i++){
        if (is_valid(game_board, grid_height, grid_width, box_height, box_width, row, col, i)){
            game_board[row][col].value = i;
            m.row = row;
            m.col = col;
            m.value = i;
            push(&s, m);

            if (col + 1>= grid_width) {
                count += _count_solutions_iterative(game_board, grid_height, grid_width, box_height, box_width, row + 1, 0, &s);
            }

            else {
                count += _count_solutions_iterative(game_board, grid_height, grid_width, box_height, box_width, row, col+1, &s);
            }
            game_board[row][col].value = UNASSIGNED;
        }
    }

    return count;
}


/**
 * Check if the board is complete (completely filled)
 * @param game_board The game board
 * @param grid_height The board height
 * @param grid_width The board width
 * @return 1 = Game was won, 0 - Game is not won.
 */
int is_board_complete(board game_board, int grid_height, int grid_width) {
    int i;
    int j;
    for (i = 0; i < grid_height; i++) {
        for (j = 0; j < grid_width; j++) {
            if (game_board[i][j].value == UNASSIGNED)
                return FALSE;
        }
    }

    return TRUE;
}


/**
 * Solve a board game game_board recursively
 * @param game_board The game board
 * @param grid_height The board height
 * @param grid_width The width of the board
 * @param box_height The box height
 * @param box_width The box width
 * @param row The row of the cell to start solving from
 * @param col The column of the cell to start solving from
 * @param flag 1 randomly 2 deterministic
 * @return 1 = Solved, 0 = No solution found
 */
int solve_grid(board game_board, int grid_height, int grid_width, int box_height, int box_width, int row,
                                 int col) {
    int num;

    if (row == grid_height)
        return TRUE;

    if (!is_empty(game_board, row, col)) {
        /* Current cell is not empty - skip it and try the next one */
        if (col + 1 < grid_width) {
            /* If we can go further right - then go there and solve from there */
            return solve_grid(game_board, grid_height, grid_width, box_height, box_width, row, col + 1);
        } else {
            /* End of the row - go to the next one and try to solve from there */
            return solve_grid(game_board, grid_height, grid_width, box_height, box_width, row + 1, 0);
        }
    }

    else {
        for (num = 1; num <= (box_height * box_width); num++) {
            /* Check if num is valid value for the current cell */
            if (is_valid(game_board, grid_height, grid_width, box_height, box_width, row, col, num)) {
                /* Fill the cell with the proposed num */
                game_board[row][col].value = num;

                if (col + 1 < grid_width) {
                    /* If we can go further right - then go there */
                    if (solve_grid(game_board, grid_height, grid_width, box_height, box_width, row, col + 1)) {
                        /* Success! */
                        return TRUE;
                    }
                }

                else {
                    /* End of the row - go to the next one */
                    if (solve_grid(game_board, grid_height, grid_width, box_height, box_width, row + 1, 0)) {
                        /* Success! */
                        return TRUE;
                    }
                }

                /* No solution found with current num as value in current cell - backtrack */
                game_board[row][col].value = UNASSIGNED;
            }
        }

        /* No solution found for current board - return False (to trigger backtracking) */
        return FALSE;
    }
}
