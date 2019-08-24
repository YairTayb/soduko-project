#ifndef NIM_CONSTS_H
#define NIM_CONSTS_H


#define EMPTY 0
#define TRUE 1
#define FALSE 0
#define MAX_COMMAND_LENGTH 256
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
#define INVALID_COMMAND_ERROR "Error: invalid command %s\n"
#define CELL_IS_FIXED_ERROR "Error: cell (%d,%d) is fixed\n"
#define CELL_IS_NOT_EMPTY_ERROR "Error: cell (%d,%d) is not empty\n"
#define INVALID_VALUE_ERROR "Error: value is invalid. Value should be in range [%d-%d]\n"
#define INVALID_INPUT_TYPE "Error: Invalid input type. %s\n"
#define WIN_MSG "Puzzle solved successfully\n"
#define SOLUTIONS_COUNT_MSG "Total number of solutions for current board: %d\n"
#define ERROR_BOARD_MSG "Board contains errors!\n"
#define HINT_MSG "Hint: set cell (%d,%d) to %d\n"
#define EXIT_MSG "Exiting...\n"
#define VALIDATION_FAILED "Validation failed: board is unsolvable\n"
#define VALIDATION_PASSED "Validation passed: board is solvable\n"
#define FUNCTION_FAILED "Error: %s has failed\n"
#define FAILED_OPENING_FILE "Error: Unable to open the file at the given path [%s]\n"
#define INVALID_COMMAND_CHOSEN "Error: Invalid command chosen - %s\n"
#define REACHED_OEF "Reached EOF while reading user input. Exiting.\n"
#define NO_ERRORS "No errors occurred.\n"
#define TOO_MANY_PARAMETERS "Too many parameters were entered. Syntax: %s\n"
#define NOT_ENOUGH_PARAMETERS "Not enough parameters were entered. Syntax: %s\n"
#define WRITING_TO_FILE_ERROR "Unable to write to file\n"
#define READIND_FROM_FILE_ERROR "Unable to read from file\n"
#define INVALID_FILE_STRUCTURE_ERROR "Invalid file structure was given. Please verify your file contents.\n"
#define ENTER_COMMAND_PROMPT "Enter command: "
#define INVALID_COMMAND_MODE "Invalid mode for command. Command is available in the following modes: %s\n"
#define CANNOT_UNDO "No moves to undo\n"
#define CANNOT_REDO "No moves to redo\n"
#define COMMAND_TOO_LONG "command is longer then 256 characters\n"
#define INVALID_BOARD "Board is invalid. Some fixed cells are conflicting\n"
#define NOT_ENOUGH_PARAMS_AMOUNT_IN_FILE "Not enough values in file. Please verify you file contents.\n"
#define TOO_MANY_PARAMS_AMOUNT_IN_FILE "Too many values in file. Please verify you file contents.\n"
#define GENERATE_X_ERROR "Number of cells to fill is larger then total number of empty cells in the board\n"
#define GENERATE_Y_ERROR "Number of cells to keep is larger then total number of cells in the board\n"
#define SAVED_SUCCESSFULL "Game was successfully to %s\n"
#define INVALID_MARK_ERRORS_VALUE "Invalid value for mark_errors. Valid values are 0, 1.\n"
#define GENERATE_EXCEEDED_ITERATIONS "Error in generate method (exceeded %d iterations)\n"
#endif
