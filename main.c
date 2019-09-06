/*created by Yair on 3.6.19*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mainaux.h"
#include "parser.h"
#include "solver.h"
#include "game.h"
#include "moves_list.h"



int main() {
    board game_board = NULL;
    int mark_errors = 1;
    int invalid_type_error_occurred = FALSE;
    int should_print_board;
    int grid_height, grid_width, box_height, box_width;
    command user_command;
    returnCodeDesc return_code_desc;
    returnCodeDesc temp_return_code_desc;
    game_mode current_mode = init_mode;
    struct MovesList game_moves;

    /*init the move list*/
    game_moves.current_move = NULL;

    printf(WELCOME_MESSAGE);

    /* Game begins */
    while (TRUE) {
        should_print_board = FALSE;
        invalid_type_error_occurred = FALSE;

        printf(ENTER_COMMAND_PROMPT);
        /* Parse the current command */
        return_code_desc = parse_command(&user_command);

        if (return_code_desc.error_code == E_INVALID_INPUT_TYPE) {
            /* An invalid param was parsed -inform later */
            temp_return_code_desc = return_code_desc;
            invalid_type_error_occurred = TRUE;
        }

        else {
            if (is_error(return_code_desc) == TRUE) {
                handle_errors(return_code_desc);

                if (return_code_desc.error_code == E_EOF_REACHED) {
                    free_whole_list(&game_moves);

                    if (game_board != NULL) {
                        free_board(game_board, grid_height);
                    }
                    exit(EXIT_FAILURE);
                }

                continue;
            }
        }

        return_code_desc = validate_command_mode(user_command, current_mode);

        if (is_error(return_code_desc) == TRUE){
            handle_errors(return_code_desc);

            if (return_code_desc.error_code == E_EOF_REACHED) {
                free_whole_list(&game_moves);

                if (game_board != NULL) {
                    free_board(game_board, grid_height);
                }
                exit(EXIT_FAILURE);
            }

            continue;
        }

        return_code_desc = validate_amount_of_parameters(user_command);

        if (is_error(return_code_desc) == TRUE){
            handle_errors(return_code_desc);

            if (return_code_desc.error_code == E_EOF_REACHED) {
                free_whole_list(&game_moves);

                if (game_board != NULL) {
                    free_board(game_board, grid_height);
                }
                exit(EXIT_FAILURE);
            }

            continue;
        } 

        if (invalid_type_error_occurred == TRUE) {
            handle_errors(temp_return_code_desc);

            if (return_code_desc.error_code == E_EOF_REACHED) {
                free_whole_list(&game_moves);

                if (game_board != NULL) {
                    free_board(game_board, grid_height);
                }
                exit(EXIT_FAILURE);
            }

            continue;
        }

        if (user_command.command_chosen == solve_command) {
            should_print_board = TRUE;

            /* Perform solve command */
            return_code_desc = solve(&game_board, user_command.path, &grid_height, &grid_width, &box_height, &box_width);

            if (is_error(return_code_desc) == FALSE) {
                current_mode = solve_mode;
                free_whole_list(&game_moves);
                init_move_list(&game_moves, grid_height, grid_width);
                add_move_to_list(game_board, &game_moves);

            } 
                

        } else if (user_command.command_chosen == edit_command) {
            should_print_board = TRUE;
            if (user_command.param_amount == 0)
                return_code_desc = edit(&game_board, user_command.path, &grid_height, &grid_width, &box_height, &box_width, FALSE);
            else
                return_code_desc = edit(&game_board, user_command.path, &grid_height, &grid_width, &box_height, &box_width, TRUE);

            if (is_error(return_code_desc) == FALSE) {
                current_mode = edit_mode;
                free_whole_list(&game_moves);
                init_move_list(&game_moves, grid_height, grid_width);
                add_move_to_list(game_board, &game_moves);
            }

        } else if (user_command.command_chosen == mark_errors_command) {
            return_code_desc = set_mark_errors(&mark_errors, user_command.params[0]);

        } else if (user_command.command_chosen == print_board_command) {
            /* Do nothing - the board will print anyway */
            should_print_board = TRUE;
            return_code_desc.error_code = E_SUCCESS;
            strcpy(return_code_desc.error_message, NO_ERRORS);

        } else if (user_command.command_chosen == set_command) {
            should_print_board = TRUE;

            return_code_desc = set(game_board, grid_height, grid_width, box_height, box_width, user_command.params[0] - 1,
                                   user_command.params[1] - 1, user_command.params[2], current_mode);

            if (is_error(return_code_desc) == FALSE) {
                /*adding the move*/
                game_board[user_command.params[0] - 1][user_command.params[1] - 1].has_changed = TRUE;/*ROW - COL*/
                add_move_to_list(game_board, &game_moves);
            }

        } else if (user_command.command_chosen == validate_command) {
            return_code_desc = validate(game_board, grid_height, grid_width, box_height, box_width);

        } else if (user_command.command_chosen == guess_command) {
            should_print_board = TRUE;

        } else if (user_command.command_chosen == generate_command) {
            should_print_board = TRUE;
            return_code_desc = generate(game_board, grid_height, grid_width, box_height, box_width,
                                        user_command.params[0],
                                        user_command.params[1]);

            if (is_error(return_code_desc) == FALSE) {
                /*adding the move*/
                add_move_to_list(game_board, &game_moves);
            }

        } else if (user_command.command_chosen == undo_command) {
            should_print_board = TRUE;

            return_code_desc = undo_move(game_board, grid_height, grid_width, &game_moves);

        } else if (user_command.command_chosen == redo_command) {
            should_print_board = TRUE;

            return_code_desc = redo_move(game_board, grid_height, grid_width, &game_moves);

        } else if (user_command.command_chosen == save_command) {
            return_code_desc = save(game_board, grid_height, grid_width, box_height, box_width, current_mode,
                                    user_command.path);

            if (is_error(return_code_desc) == FALSE){
                printf(SAVED_SUCCESSFULL, user_command.path);
            }

        } else if (user_command.command_chosen == hint_command) {
            return_code_desc = hint(game_board, grid_height, grid_width, box_height, box_width, user_command.params[0] - 1,
                                    user_command.params[1] - 1);

        } else if (user_command.command_chosen == guess_hint_command) {

        } else if (user_command.command_chosen == num_solutions_command) {
            return_code_desc = num_solutions(game_board, grid_height, grid_width, box_height, box_width);

        } else if (user_command.command_chosen == autofill_command) {
            should_print_board = TRUE;
            return_code_desc = autofill(game_board, grid_height, grid_width, box_height, box_width);

            if (is_error(return_code_desc) == FALSE) {
                /*adding the move*/
                add_move_to_list(game_board, &game_moves);
            }

        } else if (user_command.command_chosen == reset_command) {
            return_code_desc = reset_game(game_board, &game_moves);
            should_print_board = TRUE;

        } else if (user_command.command_chosen == exit_command) {
            free_whole_list(&game_moves);

            if (game_board != NULL) {
                free_board(game_board, grid_height);
            }
            exit(EXIT_SUCCESS);

        }

        if (is_error(return_code_desc) == TRUE){
            handle_errors(return_code_desc);

            if (return_code_desc.error_code == E_EOF_REACHED) {
                free_whole_list(&game_moves);

                if (game_board != NULL) {
                    free_board(game_board, grid_height);
                }
                exit(EXIT_FAILURE);
            }

            continue;
        } else {
            printf("%s",return_code_desc.error_message);
        }

        update_board_errors(game_board, grid_height, grid_width, box_height, box_width);
        reset_board_changed_status(game_board, grid_height, grid_width);
        

        if (should_print_board == TRUE) {
            print_board(game_board, grid_height, grid_width, box_height, box_width, current_mode, mark_errors);
        }

        /* Check if the game was won */
        if (current_mode == solve_mode && is_board_complete(game_board, grid_height, grid_width)) {
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
