/*created by Yair on 3.6.19*/
#include <stdio.h>
#include <stdlib.h>
#include "mainaux.h"
#include "parser.h"
#include "solver.h"
#include "game.h"
#include "moves_list.h"



int main() {
    board game_board = NULL;
    int mark_errors = 1;
    int grid_height, grid_width, box_height, box_width;
    command user_command;
    returnCodeDesc return_code_desc;
    game_mode current_mode = init_mode;
    struct MovesList *game_moves ;

    game_moves = (struct MovesList *)(malloc(sizeof(struct MovesList*)));
    
    mark_errors = 1;

    /* Game begins */
    while (TRUE) {
        printf(ENTER_COMMAND_PROMPT);
        /* Parse the current command */
        return_code_desc = parse_command(&user_command);

        if (is_error(return_code_desc) == TRUE){
            handle_errors(return_code_desc);
            /* TODO - Should we continue on error? Or print the board the then continue? */
            continue;
        }

        return_code_desc = validate_command_mode(user_command, current_mode);

        if (is_error(return_code_desc) == TRUE){
            handle_errors(return_code_desc);
            continue;
        }

        return_code_desc = validate_amount_of_parameters(user_command);

        if (is_error(return_code_desc) == TRUE){
            handle_errors(return_code_desc);
            continue;
        }

        if (user_command.command_chosen == solve_command) {
            /* Perform solve command */
            
            
            return_code_desc = solve(&game_board, user_command.path, &grid_height, &grid_width, &box_height, &box_width);
            if (return_code_desc.error_code == E_SUCCESS ){
                current_mode = solve_mode;
                /*maybe a function?*/
                game_moves->board_height = grid_height;
                game_moves->board_width = grid_width;
                game_moves->current_move = NULL;
                add_move_to_list(game_board, game_moves);
            } 
                

        } else if (user_command.command_chosen == edit_command) {
            if (user_command.param_amount == 0)
                edit(&game_board, user_command.path, &grid_height, &grid_width, &box_height, &box_width, FALSE);
            else
                edit(&game_board, user_command.path, &grid_height, &grid_width, &box_height, &box_width, TRUE);
            current_mode = edit_mode;
            /*maybe a function?*/
            game_moves->board_height = grid_height;
            game_moves->board_width = grid_width;
            game_moves->current_move = NULL;
            add_move_to_list(game_board, game_moves);

        } else if (user_command.command_chosen == mark_errors_command) {
            mark_errors = user_command.params[0];

        } else if (user_command.command_chosen == print_board_command) {
            /* Do nothing - the board will print anyway */

        } else if (user_command.command_chosen == set_command) {

            return_code_desc = set(game_board, grid_height, grid_width, box_height, box_width, user_command.params[0],
                                   user_command.params[1], user_command.params[2], current_mode);
            /*adding the move*/
            add_move_to_list(game_board, game_moves);

        } else if (user_command.command_chosen == validate_command) {
            validate(game_board, grid_height, grid_width, box_height, box_width);

        } else if (user_command.command_chosen == guess_command) {

        } else if (user_command.command_chosen == generate_command) {
            return_code_desc = generate(game_board, grid_height, grid_width, box_height, box_width,
                                        user_command.params[0],
                                        user_command.params[1]);

        } else if (user_command.command_chosen == undo_command) {

            return_code_desc = undo_move(game_board, grid_height, grid_width, game_moves);

        } else if (user_command.command_chosen == redo_command) {
            return_code_desc = redo_move(game_board, grid_height, grid_width, game_moves);

        } else if (user_command.command_chosen == save_command) {
            return_code_desc = save(game_board, grid_height, grid_width, box_height, box_width, current_mode,
                                    user_command.path);

        } else if (user_command.command_chosen == hint_command) {
            return_code_desc = hint(game_board, grid_height, grid_width, box_height, box_width, user_command.params[0],
                                    user_command.params[1]);

        } else if (user_command.command_chosen == guess_hint_command) {

        } else if (user_command.command_chosen == num_solutions_command) {

        } else if (user_command.command_chosen == autofill_command) {
            return_code_desc = autofill(game_board, grid_height, grid_width, box_height, box_width);

        } else if (user_command.command_chosen == reset_command) {

        } else if (user_command.command_chosen == exit_command) {
            exit(SUCCESS);

        }
        if (is_error(return_code_desc) == TRUE){
            handle_errors(return_code_desc);
            continue;
        }

        update_board_errors(game_board, grid_height, grid_width, box_height, box_width);
        print_board(game_board, grid_height, grid_width, box_height, box_width, current_mode, mark_errors);

        /* Check if the game was won */
        if (is_board_complete(game_board, grid_height, grid_width)) {
            /* board is fully filled */
            if (is_board_errornous(game_board, grid_height, grid_width)) {
                /* board contains errors - notify user */
                print_errornous_board_message();

            } else {
                /* board is completed and valid - game was won */
                print_winning_message();
                current_mode = init_mode;
            }
        }
    }

    return SUCCESS;

}
