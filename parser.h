#ifndef SUDOKU_PARSER_H
#define SUDOKU_PARSER_H

#define FAILURE -1
#define EMPTY 0
#define TRUE 1
#define FALSE 0
#define MAX_COMMAND_LENGTH 1024
#define MAX_PATH_LENGTH 4096

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

void write_board_to_file(struct Cell** grid, int grid_height, int grid_width, int box_width, int box_height, FILE *fd, game_mode mode_of_game);

command parse_command();
#endif
