/*created by Yair on 3.6.19*/
#include "parser.h"
#include "game.h"
#include "mainaux.h"
#include "solver.h"

#include <stdlib.h>
#include <stdio.h>




int main(int argc, char *argv[]) {
    struct Cell **board;
    int i, mark_errors;
    int grid_height, grid_width, box_height, box_width;
    command user_input;
    mark_errors = 0;
    
    game_mode curr_mode = init_mode;

    /* Game begins */
    while (TRUE) {
        /* Parse the current command */
        user_input = parse_command();
        if(user_input.command_chosen == invalid_type){
            printf(INVALID_COMMAND_CHOSEN);

        } else if (user_input.command_chosen == solve_command){
            /* Perform solve command */
            solve(&board, user_input.path, &grid_height, &grid_width, &box_height, &box_width);
        }
        else if(user_input.command_chosen == edit_command) {
            edit(&board, user_input.path, &grid_height, &grid_width, &box_height, &box_width);

        } else if(user_input.command_chosen == mark_errors_command){

        } else if(user_input.command_chosen == print_board_command){

        } else if(user_input.command_chosen == set_command){

        } else if(user_input.command_chosen == validate_command){

        } else if(user_input.command_chosen == guess_command){

        } else if(user_input.command_chosen == generate_command){

        } else if(user_input.command_chosen == undo_command){

        } else if(user_input.command_chosen == redo_command){

        } else if(user_input.command_chosen == save_command){

        } else if(user_input.command_chosen == hint_command){

        } else if(user_input.command_chosen == guess_hint_command){

        } else if(user_input.command_chosen == num_solutions_command){

        } else if(user_input.command_chosen == autofill_command){

        } else if(user_input.command_chosen == reset_command){

        } else if(user_input.command_chosen == exit_command){

        }

    }

}