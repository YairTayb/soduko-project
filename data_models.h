#ifndef NIM_DATA_MODELS_H
#define NIM_DATA_MODELS_H

#include "consts.h"

struct Node {
    struct Cell **data;
    struct Node *next;
    struct Node *prev;
};

struct MovesList {
    struct Node *current_move;
    int board_width;
    int board_height;
};

typedef enum CommandFrom {
    undo, redo, reset
} command_changed_from;


typedef struct Cell {
    int value;
    int is_const;
    int is_valid;
} cell;

typedef enum Mode {
    init_mode, edit_mode, solve_mode
} game_mode;


typedef enum Type {
    invalid_type, solve_command, edit_command, mark_errors_command,
    print_board_command, set_command, validate_command, guess_command, generate_command,
    undo_command, save_command, hint_command, guess_hint_command, num_solutions_command,
    redo_command, autofill_command, reset_command, exit_command
} command_type;


typedef struct comm {
    command_type command_chosen;
    int params[3];
    int param_amount;
    char path[MAX_PATH_LENGTH];
} command;

typedef struct Move {
    int value;
    int row;
    int col;
} move;

typedef struct Elem {
    move data;
    struct Elem *next;
} elem;

typedef struct Stack {
    int count;
    elem *top;
} stack;

typedef struct Cell **board;

typedef enum _return_code {
    E_SUCCESS = 0,
    E_ERRORNOUS_BOARD = 1,
    E_INVALID_VALUE = 2,
    E_CELL_IS_FIXED = 3,
    E_CELL_IS_NOT_EMPTY = 4,
    E_INVALID_COMMAND = 5,
    E_FUNCTION_FAILED = 6,
    E_OPEN_FILE_FAILED = 7,
    E_NO_SOLUTION = 8,
    E_GAME_WON = 9,
    E_EOF_REACHED = 10,
    E_INVALID_PARAMETERS = 11,
    E_WRITE_TO_FILE_FAILED = 12,
    E_READ_FROM_FILE_FAILED = 13,
    E_INVALID_FILE_STRUCTURE = 14,
    E_INVALID_MODE = 15,
    E_GENERAL_ERROR = 16,
    E_CANNOT_UNDO = 17,
    E_CANNOT_REDO = 18,
    E_COMMAND_TOO_LONG = 19,
    E_INVALID_BOARD = 20,
    E_INVALID_INPUT_TYPE = 21,
    E_BLANK_LINE = 22,
    E_GUROBI_FAILURE = 23,
    E_FAILED_READING_FILE = 24

} return_code;

typedef struct _return_code_desc {
    return_code error_code;
    char error_message[1000];
} returnCodeDesc;


typedef enum _linear_solving_mode {
    ILP = 0,
    LP = 1
} linear_solving_mode;

#endif
