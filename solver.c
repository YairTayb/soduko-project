#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>
#include "solver.h"
#include "mainaux.h"
#include "stack.h"

/**
 * Check whether a number is already used in a given row
 * @param grid The game board
 * @param grid_width The width of the board
 * @param row The row to search the number for
 * @param current_col The current col of the cell we want to place the number at
 * @param num The number to place
 * @return 1 = Not used, 0 = Used in row.
 */
int used_in_row(struct Cell **grid, int grid_width, int row, int current_col, int num) {
    int col;
    for (col = 0; col < grid_width; col++)
        if (current_col != col && grid[row][col].value == num)
            return TRUE;
    return FALSE;
}

/**
 * Check whether a number is already used in a given col
 * @param grid The game board
 * @param grid_height The height of the board
 * @param current_row The current row of the cell we want to place the number at
 * @param col The col to search the number for
 * @param num The number to place
 * @return 1 = Not used, 0 = Used in col.
 */
int used_in_col(struct Cell **grid, int grid_height, int current_row, int col, int num) {
    int row;
    for (row = 0; row < grid_height; row++)
        if (current_row != row && grid[row][col].value == num)
            return TRUE;
    return FALSE;
}

/**
 * Check whether a given number is already used in a given box
 * @param grid The game board
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
used_in_box(struct Cell **grid, int box_start_row, int box_start_col, int box_height, int box_width, int current_row,
            int current_col, int num) {
    int row;
    int col;
    for (row = box_start_row; row < (box_start_row + box_height); row++) {
        for (col = box_start_col; col < (box_start_col + box_width); col++) {
            if (!(current_row == row && current_col == col) &&
                grid[row][col].value == num)
                return TRUE;
        }
    }
    return FALSE;
}

/**
 * Check whether it is valid to place a number in a given location
 * @param grid The game board
 * @param grid_height The board height
 * @param grid_width The board width
 * @param box_height The box height
 * @param box_width The box width
 * @param row The row to place the number at
 * @param col The column to place the number at
 * @param num The number to place
 * @return 1 = Valid, 0 = Invalid.
 */
int is_valid(struct Cell **grid, int grid_height, int grid_width, int box_height, int box_width, int row, int col,
             int num) {
    if (num != UNASSIGNED)
        return !used_in_row(grid, grid_width, row, col, num) &&
               !used_in_col(grid, grid_height, row, col, num) &&
               !used_in_box(grid, row - row % box_height, col - col % box_width, box_height, box_width, row, col, num);
    return TRUE;
}

/**
 * Find valid values for a given cell
 * @param grid The game board
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
int
find_valid_values(struct Cell **grid, int grid_height, int grid_width, int box_height, int box_width, int row, int col,
                  int *values, int range) {
    int num;
    int counter = 0;
    int i = 0;
    for (num = 1; num <= range; num++) {
        /* For each number up to the range bound - check if its valid */
        if (is_valid(grid, grid_height, grid_width, box_height, box_width, row, col, num)) {
            values[i] = num;
            i++;
            counter++;
        }
    }

    return counter;
}

/**
 * Check whether the cell is empty
 * @param grid The game board
 * @param row The row of the cell
 * @param col The column of the cell
 * @return 1 = Empty, 0 = Filled
 */
int is_empty(struct Cell **grid, int row, int col) {
    if (grid[row][col].value == UNASSIGNED)
        return TRUE;
    return FALSE;
}

/**
 * Check whether the cell is valid
 * @param grid The game board
 * @param row The row of the cell
 * @param col The column of the cell
 * @return 1 = Empty, 0 = Filled
 */
int is_cell_valid(struct Cell **grid, int row, int col) {
    if (grid[row][col].is_valid == TRUE)
        return TRUE;
    return FALSE;
}

/**
 * Given an array and an index, remove the number at the index from the array.
 * @param arr The array to remove the number from
 * @param index The index of the number to remove
 * @param arr_length The length of the array
 */
void delete_from_array(int *arr, int index, int arr_length) {
    int c;
    for (c = index; c < arr_length - 1; c++) {
        arr[c] = arr[c + 1];
    }
}


/**
 * Count the number of solutions of a given board
 * @param grid The game board
 * @param grid_height The board height
 * @param grid_width The width of the board
 * @param box_height The box height
 * @param box_width The box width
 * @param row The row of the cell to start solving from
 * @param col The column of the cell to start solving from
 * @return 1 = Solved, 0 = No solution found
 */
int count_solutions(struct Cell **grid, int grid_height, int grid_width, int box_height, int box_width, int row, int col) {
    int count = 0;
    int i;

    if (row >= grid_height) {
        /* Went through the whole board and didnt solve the sudoku. */
        return 1;
    }
    /* Boundaries check */
    if (col >= grid_width) {
        return count_solutions(grid, grid_height, grid_width, box_height, box_width, row + 1, 0);
    }

    /* Correct solution of a board */

    if (grid[row][col].is_const == TRUE || grid[row][col].value != UNASSIGNED) {
        /* We are within a legitimate cell. */
        return count_solutions(grid, grid_height, grid_width, box_height, box_width, row, col + 1);
    }

    /*maybe width?*/
    for (i = 1; i <= (box_height * box_width); i++) {
        if (is_valid(grid, grid_height, grid_width, box_height, box_width, row, col, i)) {

            grid[row][col].value = i;
            count += count_solutions(grid, grid_height, grid_width, box_height, box_width, row, col + 1);
            grid[row][col].value = UNASSIGNED;

        }
    }

    return count;
}

/**
 * Check if a game was won
 * @param grid The game board
 * @param grid_height The board height
 * @param grid_width The board width
 * @return 1 = Game was won, 0 - Game is not won.
 */
int is_game_won(struct Cell **grid, int grid_height, int grid_width) {
    int i;
    int j;
    for (i = 0; i < grid_height; i++) {
        for (j = 0; j < grid_width; j++) {
            if (grid[i][j].value == UNASSIGNED)
                return FALSE;
        }
    }

    return TRUE;
}



int main(){
    struct Cell **board;
    int sol_count = 0;
    board = create_empty_board(6, 6);
    print_board(board, 6, 6, 2, 3);
    sol_count = count_solutions(board, 6, 6, 2, 3, 0, 0);
    printf("%d\n", sol_count);
    return 1;
}
