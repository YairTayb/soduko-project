#ifndef NIM_CONSTS_H
#define NIM_CONSTS_H


#define EMPTY 0
#define TRUE 1
#define FALSE 0
#define MAX_COMMAND_LENGTH 258
#define MAX_PATH_LENGTH 4096
#define BUFFER_SIZE 1024
#define MAX_ITERATIONS_COUNTER 1000

#define FAILURE -1
#define SUCCESS 1
#define GAME_WON 1
#define COMMAND_COMPLETED 0
#define COMMAND_INCOMPLETE -1
#define ERRORNOUS_BOARD -2

#define UNASSIGNED 0
#define DEFAULT_GRID_HEIGHT 9
#define DEFAULT_GRID_WIDTH 9
#define DEFAULT_BOX_HEIGHT 3
#define DEFAULT_BOX_WIDTH 3
#define CHARACTERS_IN_CELL 3


/* Formatting & output prints */
#define WELCOME_MESSAGE "Sudoku Game\n"
#define EMPTY_CELL "    "
#define NORMAL_CELL " %2d "
#define ERROR_CELL " %2d*"
#define CONST_CELL " %2d."
#define INVALID_NUMBER_OF_CELLS_TO_FILL "Error: invalid number of cells to fill (should be between 0 and 80)\n"
#define NUMBER_OF_CELLS_TO_FILL_MSG "Please enter the number of cells to fill [0-80]:\n"
#define INVALID_COMMAND_ERROR "Error: invalid command %s. Valid commands are: solve, edit, set, mark_errors, print_board, validate, undo, redo, reset, save, autofill, num_solutions, generate, guess, hint, guess_hint, exit\n"
#define CELL_IS_FIXED_ERROR "Error: cell (%d,%d) is fixed\n"
#define CELL_IS_NOT_EMPTY_ERROR "Error: cell (%d,%d) is not empty\n"
#define INVALID_VALUE_ERROR "Error: value is invalid. Value should be in range [%d,%d]\n"
#define INVALID_DOUBLE_VALUE_ERROR "Error: value is invalid. Value should be in range (%f,%f)\n"
#define INVALID_INPUT_TYPE "Error: Invalid input type. %s\n"
#define WIN_MSG "Puzzle solved successfully. Mode changed to Init.\n"
#define SOLUTIONS_COUNT_MSG "Total number of solutions for current board: %d\n"
#define ERROR_BOARD_MSG "Error: Board contains errors!\n"
#define HINT_MSG "Set cell (%d,%d) to %d\n"
#define EXIT_MSG "Exiting..\n"
#define VALIDATION_FAILED "Error: Validation failed. board is unsolvable\n"
#define VALIDATION_PASSED "Validation passed: board is solvable\n"
#define FUNCTION_FAILED "Error: %s has failed\n"
#define FAILED_OPENING_FILE "Error: Unable to open the file at the given path [%s]\n"
#define REACHED_OEF "Error: Reached EOF while reading user input\n"
#define NO_ERRORS ""
#define TOO_MANY_PARAMETERS "Error: Too many parameters were entered. Syntax: %s\n"
#define NOT_ENOUGH_PARAMETERS "Error: Not enough parameters were entered. Syntax: %s\n"
#define WRITING_TO_FILE_ERROR "Error: Unable to write to file\n"
#define READIND_FROM_FILE_ERROR "Error: Unable to read from file\n"
#define INVALID_FILE_STRUCTURE_ERROR "Error: Invalid file structure was given. Please verify your file contents\n"
#define ENTER_COMMAND_PROMPT "Enter command: "
#define INVALID_COMMAND_MODE "Error: Invalid mode for command. Command is available in the following modes: %s\n"
#define CANNOT_UNDO "Error: No moves to undo\n"
#define CANNOT_REDO "Error: No moves to redo\n"
#define COMMAND_TOO_LONG "Error: command exceeds max length of %d characters. Skipping command\n"
#define INVALID_BOARD "Error: Board is invalid. Some fixed cells are conflicting\n"
#define NOT_ENOUGH_PARAMS_AMOUNT_IN_FILE "Error: Not enough values in file. Please verify you file contents\n"
#define TOO_MANY_PARAMS_AMOUNT_IN_FILE "Error: Too many values in file. Please verify you file contents\n"
#define GENERATE_X_ERROR "Error: Number of cells to fill is larger then total number of empty cells in the board\n"
#define GENERATE_Y_ERROR "Error: Number of cells to keep is larger then total number of cells in the board\n"
#define SAVED_SUCCESSFULL "Game was successfully saved to %s\n"
#define INVALID_MARK_ERRORS_VALUE "Error: Invalid value for mark_errors. Valid values are 0, 1\n"
#define GENERATE_EXCEEDED_ITERATIONS "Error: generate method exceeded %d iterations\n"
#define FILE_PARAMS_NOT_IN_RANGE "Error: some file parameters are not in the legal range. Value should be in range [%d-%d]\n"
#define FILE_PARAMS_NOT_LEGAL "Error: some file parameters are not legal\n"
#define BOARD_IS_ERRONOUS "Error: board is erronous, generate could not generate any value\n"
#define READING_FROM_FILE "Error: could not read from the file. \n"
#define INVALID_FLOAT_PARAM "Error: Param X should be a valid float number\n"
#define SOLUTION_WRONG "Error: The solution of the board is wrong\n"
#define GUESS_HINT_MSG "Cell (%d,%d) - VALUE: %d, SCORE: %f\n"

#define GUROBI_NO_SOLUTION_ERROR "Error: No solution found using Gurobi\n"
#endif
