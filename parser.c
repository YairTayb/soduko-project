#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mainaux.h"
#include "parser.h"


int string_to_int(char *number ){
    int i, num;
    i = 0;
    num = 0;
    /*check length before*/
    while(number[i] != '.' && number[i] && number[i] != EOF){
        num = num * 10;
        num += number[i] -'0';
        i++;
    }
    return num;


}

returnCodeDesc new_string_to_int(char *number,int *result){
    int i = 0;
    returnCodeDesc return_code_desc;
    *result = 0;
    while (number[i]){
        if(number[i] < '0' || number[i] >'9')
        {
            return_code_desc.error_code = E_INVALID_INPUT_TYPE;
            sprintf(return_code_desc.error_message, INVALID_INPUT_TYPE, "Input should be a number");
            return return_code_desc;
        }
            
        *result = (*result) * 10;
        *result += number[i] -'0';
        i++;
    }

    return_code_desc.error_code = E_SUCCESS;
    strcpy(return_code_desc.error_message, NO_ERRORS);
    return return_code_desc;;

}




int check_if_const(char *number){
    int i;
    i = strlen(number);
    if(i <= 1){
        return FALSE;
    }
    if(number[i-1] == '.' ){
        return TRUE;
    }
    return FALSE;
}

command parse_command_name(char* command_name) {
    command received_command;

    if (strcmp(command_name, "solve") == 0) {
        received_command.command_chosen = solve_command;

    } else if (strcmp(command_name, "edit") == 0) {
        received_command.command_chosen = edit_command;

    } else if (strcmp(command_name, "mark_errors") == 0) {
        received_command.command_chosen = mark_errors_command;

    } else if (strcmp(command_name, "print_board") == 0) {
        received_command.command_chosen = print_board_command;

    } else if (strcmp(command_name, "set") == 0) {
        received_command.command_chosen = set_command;

    } else if (strcmp(command_name, "validate") == 0) {
        received_command.command_chosen = validate_command;

    } else if (strcmp(command_name, "guess") == 0) {
        received_command.command_chosen = guess_command;

    } else if (strcmp(command_name, "generate") == 0) {
        received_command.command_chosen = generate_command;

    } else if (strcmp(command_name, "undo") == 0) {
        received_command.command_chosen = undo_command;

    } else if (strcmp(command_name, "redo") == 0) {
        received_command.command_chosen = redo_command;

    } else if (strcmp(command_name, "save") == 0) {
        received_command.command_chosen = save_command;

    } else if (strcmp(command_name, "hint") == 0) {
        received_command.command_chosen = hint_command;

    } else if (strcmp(command_name, "guess_hint") == 0) {
        received_command.command_chosen = guess_hint_command;

    } else if (strcmp(command_name, "num_solutions") == 0) {
        received_command.command_chosen = num_solutions_command;

    } else if (strcmp(command_name, "autofill") == 0) {
        received_command.command_chosen = autofill_command;

    } else if (strcmp(command_name, "reset") == 0) {
        received_command.command_chosen = reset_command;

    } else if (strcmp(command_name, "exit") == 0) {
        received_command.command_chosen = exit_command;

    } else {
        /*invalid command entered*/
        received_command.command_chosen = invalid_type;
    }

    return received_command;
}


returnCodeDesc validate_command_mode(command user_command, game_mode current_mode) {
    returnCodeDesc return_code_desc;
    return_code_desc.error_code = E_SUCCESS;
    strcpy(return_code_desc.error_message, NO_ERRORS);

    switch (user_command.command_chosen) {
        case mark_errors_command:
            if (current_mode != solve_mode) {
                return_code_desc.error_code = E_INVALID_MODE;
                sprintf(return_code_desc.error_message, INVALID_COMMAND_MODE, "Solve");
            }
            break;
        case print_board_command:
            if (current_mode != solve_mode && current_mode != edit_mode) {
                return_code_desc.error_code = E_INVALID_MODE;
                sprintf(return_code_desc.error_message, INVALID_COMMAND_MODE, "Solve, Edit");
            }
            break;
        case set_command:
            if (current_mode != solve_mode && current_mode != edit_mode) {
                return_code_desc.error_code = E_INVALID_MODE;
                sprintf(return_code_desc.error_message, INVALID_COMMAND_MODE, "Solve, Edit");
            }
            break;
        case validate_command:
            if (current_mode != solve_mode && current_mode != edit_mode) {
                return_code_desc.error_code = E_INVALID_MODE;
                sprintf(return_code_desc.error_message, INVALID_COMMAND_MODE, "Solve, Edit");
            }
            break;
        case guess_command:
            if (current_mode != solve_mode) {
                return_code_desc.error_code = E_INVALID_MODE;
                sprintf(return_code_desc.error_message, INVALID_COMMAND_MODE, "Solve");
            }
            break;
        case generate_command:
            if (current_mode != edit_mode) {
                return_code_desc.error_code = E_INVALID_MODE;
                sprintf(return_code_desc.error_message, INVALID_COMMAND_MODE, "Edit");
            }
            break;
        case undo_command:
            if (current_mode != solve_mode && current_mode != edit_mode) {
                return_code_desc.error_code = E_INVALID_MODE;
                sprintf(return_code_desc.error_message, INVALID_COMMAND_MODE, "Solve, Edit");
            }
            break;
        case redo_command:
            if (current_mode != solve_mode && current_mode != edit_mode) {
                return_code_desc.error_code = E_INVALID_MODE;
                sprintf(return_code_desc.error_message, INVALID_COMMAND_MODE, "Solve, Edit");
            }
            break;
        case save_command:
            if (current_mode != solve_mode && current_mode != edit_mode) {
                return_code_desc.error_code = E_INVALID_MODE;
                sprintf(return_code_desc.error_message, INVALID_COMMAND_MODE, "Solve, Edit");
            }
            break;
        case hint_command:
            if (current_mode != solve_mode) {
                return_code_desc.error_code = E_INVALID_MODE;
                sprintf(return_code_desc.error_message, INVALID_COMMAND_MODE, "Solve");
            }
            break;
        case guess_hint_command:
            if (current_mode != solve_mode && current_mode != edit_mode) {
                return_code_desc.error_code = E_INVALID_MODE;
                sprintf(return_code_desc.error_message, INVALID_COMMAND_MODE, "Solve, Edit");
            }
            break;
        case num_solutions_command:
            if (current_mode != solve_mode && current_mode != edit_mode) {
                return_code_desc.error_code = E_INVALID_MODE;
                sprintf(return_code_desc.error_message, INVALID_COMMAND_MODE, "Solve, Edit");
            }
            break;
        case autofill_command:
            if (current_mode != solve_mode) {
                return_code_desc.error_code = E_INVALID_MODE;
                sprintf(return_code_desc.error_message, INVALID_COMMAND_MODE, "Solve");
            }
            break;
        case reset_command:
            if (current_mode != solve_mode && current_mode != edit_mode) {
                return_code_desc.error_code = E_INVALID_MODE;
                sprintf(return_code_desc.error_message, INVALID_COMMAND_MODE, "Solve, Edit");
            }
            break;
        default:
            break;
    }

    return return_code_desc;

}


returnCodeDesc  validate_amount_of_parameters(command user_command) {
    returnCodeDesc return_code_desc;
    return_code_desc.error_code = E_SUCCESS;
    strcpy(return_code_desc.error_message, NO_ERRORS);

    switch (user_command.command_chosen) {
        case solve_command:
            if (user_command.param_amount > 1) {
                return_code_desc.error_code = E_INVALID_PARAMETERS;
                sprintf(return_code_desc.error_message, TOO_MANY_PARAMETERS, "solve X");
            } else if (user_command.param_amount < 1) {
                return_code_desc.error_code = E_INVALID_PARAMETERS;
                sprintf(return_code_desc.error_message, NOT_ENOUGH_PARAMETERS, "solve X");
            }
            break;

        case edit_command:
            if (user_command.param_amount > 1) {
                return_code_desc.error_code = E_INVALID_PARAMETERS;
                sprintf(return_code_desc.error_message, TOO_MANY_PARAMETERS, "edit [X]");
            }

            break;
        case mark_errors_command:
            if (user_command.param_amount > 1) {
                return_code_desc.error_code = E_INVALID_PARAMETERS;
                sprintf(return_code_desc.error_message, TOO_MANY_PARAMETERS, "mark_errors X");
            } else if (user_command.param_amount < 1) {
                return_code_desc.error_code = E_INVALID_PARAMETERS;
                sprintf(return_code_desc.error_message, NOT_ENOUGH_PARAMETERS, "mark_errors X");
            }
            break;
        case print_board_command:
            if (user_command.param_amount > 0) {
                return_code_desc.error_code = E_INVALID_PARAMETERS;
                sprintf(return_code_desc.error_message, TOO_MANY_PARAMETERS, "print_board");
            }
            break;
        case set_command:
            if (user_command.param_amount > 3) {
                return_code_desc.error_code = E_INVALID_PARAMETERS;
                sprintf(return_code_desc.error_message, TOO_MANY_PARAMETERS, "set X Y Z");
            } else if (user_command.param_amount < 3) {
                return_code_desc.error_code = E_INVALID_PARAMETERS;
                sprintf(return_code_desc.error_message, NOT_ENOUGH_PARAMETERS, "set X Y Z");
            }
            break;
        case validate_command:
            if (user_command.param_amount > 0) {
                return_code_desc.error_code = E_INVALID_PARAMETERS;
                sprintf(return_code_desc.error_message, TOO_MANY_PARAMETERS, "validate");
            }
            break;
        case guess_command:
            if (user_command.param_amount > 1) {
                return_code_desc.error_code = E_INVALID_PARAMETERS;
                sprintf(return_code_desc.error_message, TOO_MANY_PARAMETERS, "guess X");
            } else if (user_command.param_amount < 1) {
                return_code_desc.error_code = E_INVALID_PARAMETERS;
                sprintf(return_code_desc.error_message, NOT_ENOUGH_PARAMETERS, "guess X");
            }
            break;
        case generate_command:
            if (user_command.param_amount > 2) {
                return_code_desc.error_code = E_INVALID_PARAMETERS;
                sprintf(return_code_desc.error_message, TOO_MANY_PARAMETERS, "generate X Y");
            } else if (user_command.param_amount < 2) {
                return_code_desc.error_code = E_INVALID_PARAMETERS;
                sprintf(return_code_desc.error_message, NOT_ENOUGH_PARAMETERS, "generate X Y");
            }
            break;
        case undo_command:
            if (user_command.param_amount > 0) {
                return_code_desc.error_code = E_INVALID_PARAMETERS;
                sprintf(return_code_desc.error_message, TOO_MANY_PARAMETERS, "undo");
            }
            break;
        case redo_command:
            if (user_command.param_amount > 0) {
                return_code_desc.error_code = E_INVALID_PARAMETERS;
                sprintf(return_code_desc.error_message, TOO_MANY_PARAMETERS, "redo");
            }
            break;
        case save_command:
            if (user_command.param_amount > 1) {
                return_code_desc.error_code = E_INVALID_PARAMETERS;
                sprintf(return_code_desc.error_message, TOO_MANY_PARAMETERS, "save X");
            } else if (user_command.param_amount < 1) {
                return_code_desc.error_code = E_INVALID_PARAMETERS;
                sprintf(return_code_desc.error_message, NOT_ENOUGH_PARAMETERS, "save X");
            }
            break;
        case hint_command:
            if (user_command.param_amount > 2) {
                return_code_desc.error_code = E_INVALID_PARAMETERS;
                sprintf(return_code_desc.error_message, TOO_MANY_PARAMETERS, "hint X Y");
            } else if (user_command.param_amount < 2) {
                return_code_desc.error_code = E_INVALID_PARAMETERS;
                sprintf(return_code_desc.error_message, NOT_ENOUGH_PARAMETERS, "hint X Y");
            }
            break;
        case guess_hint_command:
            if (user_command.param_amount > 2) {
                return_code_desc.error_code = E_INVALID_PARAMETERS;
                sprintf(return_code_desc.error_message, TOO_MANY_PARAMETERS, "guess_hint X Y");
            } else if (user_command.param_amount < 2) {
                return_code_desc.error_code = E_INVALID_PARAMETERS;
                sprintf(return_code_desc.error_message, NOT_ENOUGH_PARAMETERS, "guess_hint X Y");
            }
            break;
        case num_solutions_command:
            if (user_command.param_amount > 0) {
                return_code_desc.error_code = E_INVALID_PARAMETERS;
                sprintf(return_code_desc.error_message, TOO_MANY_PARAMETERS, "num_solutions");
            }
            break;
        case autofill_command:
            if (user_command.param_amount > 0) {
                return_code_desc.error_code = E_INVALID_PARAMETERS;
                sprintf(return_code_desc.error_message, TOO_MANY_PARAMETERS, "autofill");
            }
            break;
        case reset_command:
            if (user_command.param_amount > 0) {
                return_code_desc.error_code = E_INVALID_PARAMETERS;
                sprintf(return_code_desc.error_message, TOO_MANY_PARAMETERS, "reset");
            }
            break;
        case exit_command:
            if (user_command.param_amount > 0) {
                return_code_desc.error_code = E_INVALID_PARAMETERS;
                sprintf(return_code_desc.error_message, TOO_MANY_PARAMETERS, "exit");
            }
            break;
        default:
            break;
    }

    return return_code_desc;

}

/**
 * Parse a game command from input stream. A command that receives an input and returns a command struct
 *containing the parsed command from the user.
 * @return {Command} The parsed command struct.
 */
returnCodeDesc parse_command(command *user_command){
    returnCodeDesc return_code_desc;
    char *result;
    int c;
    int invalid_input_type_error_occurred=FALSE;
    char user_input[MAX_COMMAND_LENGTH];
    char *token;
    int num_recieved;

    /* Reached EOF - Exit! */
    if (feof(stdin)) {
        return_code_desc.error_code = E_EOF_REACHED;
        strcpy(return_code_desc.error_message, REACHED_OEF);
        return return_code_desc;
    }

    /* User input as a whole row. */
    result = fgets(user_input, MAX_COMMAND_LENGTH, stdin);

    /* Check what caused the fgets to fail */
    if (result == NULL) {
        if (feof(stdin)) {
            /* Reached EOF */
            return_code_desc.error_code = E_EOF_REACHED;
            strcpy(return_code_desc.error_message, REACHED_OEF);
            return return_code_desc;

        } else if (ferror(stdin)) {
            return_code_desc.error_code = E_FUNCTION_FAILED;
            sprintf(return_code_desc.error_message, FUNCTION_FAILED, "fgets");
            return return_code_desc;

        }
    }
    /* Verify command length - not exceeding MAX_COMMAND_LENGTH of 258. According to Moshe, command shouldn't exceed
     * 256 chars (not including \n and \0)*/
    if (strchr(user_input, '\n') == NULL){

            return_code_desc.error_code = E_COMMAND_TOO_LONG;
            sprintf(return_code_desc.error_message, COMMAND_TOO_LONG, MAX_COMMAND_LENGTH - 2);

            /*clean stdin*/
            while ((c = getchar()) != '\n' && c != EOF);

            if (c == EOF) {
                return_code_desc.error_code = E_EOF_REACHED;
                strcpy(return_code_desc.error_message, REACHED_OEF);
                return return_code_desc;
            }

            return return_code_desc;
    }


    /* Skip the empty lines */
    /* TODO: Verify in the instructions if we need to wait for input, or skip the empty line and prompt for command again */
    if (is_empty_string(user_input) == TRUE) {
        return_code_desc.error_code = E_BLANK_LINE;
        return return_code_desc;
    }

    /* Remove trailing \n in the command input */
    if (*user_input && user_input[strlen(user_input) - 1] == '\n')
        user_input[strlen(user_input) - 1] = '\0';

    /* Split the input */
    token = strtok(user_input, " ");

    /* Finished screening for white spaces and EOF, start screening the command */
    /* Identifying the command entered. */
    *user_command = parse_command_name(token);


    /* Verify the command is valid */
    if ((*user_command).command_chosen == invalid_type) {
        return_code_desc.error_code = E_INVALID_COMMAND;
        sprintf(return_code_desc.error_message, INVALID_COMMAND_ERROR, token);
        return return_code_desc;
    }

    (*user_command).param_amount = 0;

    /* Tokenizing and counting parameters */
    token = strtok(NULL," ");

    while(token != NULL){
        if (invalid_input_type_error_occurred == FALSE) {
            /* whether we need to save a path or a parameter */
            if ((*user_command).command_chosen == edit_command ||
                (*user_command).command_chosen == save_command || (*user_command).command_chosen == solve_command) {
                     strcpy((*user_command).path, token);
            }

            else if ((*user_command).command_chosen == guess_hint_command) {
                /* Parse float */
            }

            else {

                return_code_desc = new_string_to_int(token, &num_recieved);

                if (is_error(return_code_desc) == FALSE) {
                    if ((*user_command).param_amount < 3)
                        /* PREVENT BUFFER OVERFLOW */
                        (*user_command).params[(*user_command).param_amount] = num_recieved;
                } else {
                    switch ((*user_command).param_amount) {
                        case 0:
                            sprintf(return_code_desc.error_message, INVALID_INPUT_TYPE, "Param X should be a positive integer");
                            break;
                        case 1:
                            sprintf(return_code_desc.error_message, INVALID_INPUT_TYPE, "Param Y should be a positive integer");
                            break;
                        case 2:
                            sprintf(return_code_desc.error_message, INVALID_INPUT_TYPE, "Param Z should be a positive integer");
                            break;
                        default:
                            break;
                    }
                    invalid_input_type_error_occurred = TRUE;
                }
            }
        }

        (*user_command).param_amount++;
        token = strtok(NULL, " \t\r\n");
    }

    if (invalid_input_type_error_occurred == TRUE)
        return return_code_desc;

    return_code_desc.error_code = E_SUCCESS;
    strcpy(return_code_desc.error_message, NO_ERRORS);
    return return_code_desc;
}


returnCodeDesc
write_board_to_file(struct Cell **grid, int grid_height, int grid_width, int box_width, int box_height, FILE *fd,
                    game_mode mode_of_game) {

    int i, j, token;
    returnCodeDesc return_code_desc;

    token = fprintf(fd, "%d %d\n", box_height, box_width);
    if (token < 0) {
        return_code_desc.error_code = E_WRITE_TO_FILE_FAILED;
        strcpy(return_code_desc.error_message, WRITING_TO_FILE_ERROR);
        return return_code_desc;
    }

    for (i = 0; i < grid_height; i++) {
        for (j = 0; j < grid_width; j++) {
            
            if(j == grid_height-1){

                if (mode_of_game == edit_mode) {

                    if (grid[i][j].value == UNASSIGNED) {
                        token = fprintf(fd, "%d", grid[i][j].value);
                    } else {
                        token = fprintf(fd, "%d.", grid[i][j].value);
                    }
                } else {
                    if (grid[i][j].is_const == TRUE) {
                        token = fprintf(fd, "%d.", grid[i][j].value);

                    } else {
                        token = fprintf(fd, "%d", grid[i][j].value);
                    }
                }
            } else {
                if (mode_of_game == edit_mode) {
                    if (grid[i][j].value == EMPTY) {
                        token = fprintf(fd, "%d ", grid[i][j].value);
                    } else {
                        token = fprintf(fd, "%d. ", grid[i][j].value);
                    }
                } else {
                    if (grid[i][j].is_const == TRUE) {
                        token = fprintf(fd, "%d. ", grid[i][j].value);

                    } else {
                        token = fprintf(fd, "%d ", grid[i][j].value);
                    }
                }
            }

            
            if (token < 0) {
                return_code_desc.error_code = E_WRITE_TO_FILE_FAILED;
                strcpy(return_code_desc.error_message, WRITING_TO_FILE_ERROR);
                return return_code_desc;
            }
        }

        /*preventing adding another new line in the end of the file.*/
        if(i != grid_height-1){
            token = fprintf(fd, "\n");
        }

        if (token < 0) {
            return_code_desc.error_code = E_WRITE_TO_FILE_FAILED;
            strcpy(return_code_desc.error_message, WRITING_TO_FILE_ERROR);
            return return_code_desc;
        }
    }

    return_code_desc.error_code = E_SUCCESS;
    strcpy(return_code_desc.error_message, NO_ERRORS);
    return return_code_desc;
}

int is_valid_number(char *num){
    int dot_flag;
    int i = 0;
    dot_flag = FALSE;

    while(num[i]){

        if(num[i] < '0' || num[i] > '9'){

            if(num[i] == '.' && !dot_flag){

                dot_flag = TRUE;

                if(i == ((int)strlen(num) - 1)){
                    return TRUE;
                } else {
                    return FALSE;
                }

            } else {
                return FALSE;
            }
        }
        i++;
    }

    return TRUE;
}
int is_numeric(char *num){
    int i = 0;
    while(num[i]){

        if(num[i] < '0' || num[i] > '9'){
            return FALSE;
        }
        i++;
    }

    return TRUE;
}


returnCodeDesc read_board_from_file(FILE *fd, board *grid_pointer, int *grid_height_pointer, int *grid_width_pointer,
                         int *box_height_pointer,
                         int *box_width_pointer) {
    int rows_amount, columns_amount, total_length;
    int values_read_amount, curr_val;
    int cur_row, cur_col;
    char read_buffer[BUFFER_SIZE];
    char *tok;
    int read_rows, read_cols;
    returnCodeDesc ret_val;
    read_rows = FALSE;
    read_cols = FALSE;

    /*parsing the file into the board*/
    values_read_amount = 0;

    /* Initialize the buffer to zeros */
    memset(read_buffer, 0, BUFFER_SIZE);

    /*reading as much as we can*/
    /* TODO: Add error handling for the fread(). Seems that if the ret val of fread != to BUGGER_SIZE - 1 then an error or EOF happend */
    while (fread(read_buffer, sizeof(char), BUFFER_SIZE - 1, fd) > 0) {
        tok = strtok(read_buffer, " \t\r\n");
        values_read_amount++;
        /*parsing while we can*/
        while (tok) {
            if (read_rows == FALSE) {
                if(is_numeric(tok)){
                    rows_amount = string_to_int(tok);/* */
                    *box_height_pointer = rows_amount;
                    read_rows = TRUE;

                } else {
                    ret_val.error_code = E_INVALID_FILE_STRUCTURE;
                    strcpy(ret_val.error_message,INVALID_FILE_STRUCTURE_ERROR);
                    return ret_val;
                }
                
            } else if (read_cols == FALSE) {

                if(is_numeric(tok)){

                    columns_amount = string_to_int(tok);/* */
                    total_length = rows_amount * columns_amount;
                    *grid_pointer = create_empty_board(total_length, total_length);

                    *box_width_pointer = columns_amount;
                    read_cols = TRUE;

                } else {

                    ret_val.error_code = E_INVALID_FILE_STRUCTURE;
                    strcpy(ret_val.error_message,INVALID_FILE_STRUCTURE_ERROR);
                    return ret_val;
                }
                
            } else {
                *grid_height_pointer = total_length;
                *grid_width_pointer = total_length;
                cur_row = (values_read_amount -1) / total_length;
                
                if ((values_read_amount ) % total_length == 0) {
                    cur_col = total_length - 1;
                } else {
                    cur_col = (values_read_amount) % total_length - 1;
                }

                if(is_valid_number(tok)){
                    curr_val = string_to_int(tok);
                    
                    if(!(curr_val >= 0 && curr_val <= total_length))
                    {
                        /* TODO: Maybe change to more informative error? regarding the valid range */
                        ret_val.error_code = E_INVALID_FILE_STRUCTURE;
                        strcpy(ret_val.error_message,INVALID_FILE_STRUCTURE_ERROR);
                        free_board(*grid_pointer, total_length);
                        return ret_val;
                    }
                } else {
                    ret_val.error_code = E_INVALID_FILE_STRUCTURE;
                    strcpy(ret_val.error_message,INVALID_FILE_STRUCTURE_ERROR);
                    tok[strlen(tok) - 1] = '\0';
                    free_board(*grid_pointer, total_length);
                    return ret_val;
                }

                if(values_read_amount > total_length*total_length){

                    ret_val.error_code = E_INVALID_FILE_STRUCTURE;
                    strcpy(ret_val.error_message,TOO_MANY_PARAMS_AMOUNT_IN_FILE);
                    free_board(*grid_pointer, total_length);
                    return ret_val;
                }

                (*grid_pointer)[cur_row][cur_col].value = curr_val;
                (*grid_pointer)[cur_row][cur_col].is_const = check_if_const(tok);
                (*grid_pointer)[cur_row][cur_col].is_valid = 1;
                values_read_amount++;
            }

            tok = strtok(NULL, " \t\r\n");
            
        }

        memset(read_buffer, 0, BUFFER_SIZE);
    }

    if(values_read_amount -1 < total_length*total_length){

        ret_val.error_code = E_INVALID_FILE_STRUCTURE;
        strcpy(ret_val.error_message,NOT_ENOUGH_PARAMS_AMOUNT_IN_FILE);
        free_board(*grid_pointer, total_length);
        return ret_val;
    }



    ret_val.error_code = E_SUCCESS;
    strcpy(ret_val.error_message,NO_ERRORS);
    return ret_val;

}

/*
int main(){
    int num = 0;
    char *check = "a11b";
    new_string_to_int(check,&num);
    printf("%d",num);


}*/


/*
int main(){
    int i;
    int count = 15;
    command test_command;*/
    /*char string_board[4096] = {"1. 0 1  \n 1 1 1\n 0. 0 0 \n"};
    string_to_board(NULL,3,3,string_board);*/
    /*
    while(count >= 0){
        test_command= parse_command();
        if(test_command.command_chosen == set_command){
            printf("set command.\n");
        }

        for(i = 0; i < 3; i++){
            printf("param %d: %d,",i, test_command.params[i]);
        }
        printf("param amount is:%d\n",test_command.param_amount );
        printf("\n");
        printf("path is:%s \n", test_command.path);
        count--;
    }*/
    /*
    i = string_to_int("11.");
    printf("%d \n", i);
    i = string_to_int("11");
    printf("%d \n", i);
    i = string_to_int("9.");
    printf("%d \n", i);
    i = string_to_int("9");
    printf("%d \n", i);
    i = string_to_int(".");
    printf("%d \n", i);
    i = string_to_int("0");
    printf("%d \n", i);*/

    /*
    int int_grid[9][9] = {{4, 0, 1, 0, 0, 0, 0, 5, 2},
                          {0, 0, 0, 0, 0, 0, 0, 0, 0},
                          {0, 5, 0, 0, 4, 1, 0, 0, 0},
                          {0, 0, 0, 2, 0, 0, 5, 4, 0},
                          {0, 1, 0, 4, 0, 0, 0, 2, 3},
                          {0, 4, 2, 0, 0, 0, 1, 0, 0},
                          {0, 0, 4, 0, 0, 0, 0, 0, 0},
                          {0, 7, 0, 1, 0, 0, 0, 0, 4},
                          {0, 0, 0, 7, 0, 0, 0, 0, 0}};
    struct Cell **grid = (struct Cell **) malloc(DEFAULT_GRID_HEIGHT * sizeof(struct Cell));
    int i;
    int j;*/
    /*
    if(!grid){
        printf(FUNCTION_FAILED, "malloc");
        exit(0);
    }

    for (i = 0; i < DEFAULT_GRID_HEIGHT; i++){
        grid[i] = (struct Cell *) malloc(DEFAULT_GRID_WIDTH * sizeof(struct Cell));
        
        if(!grid[i]){
            printf(FUNCTION_FAILED, "malloc");
            exit(0);
        }

    }
    
    for (i = 0; i < DEFAULT_GRID_HEIGHT; i++) {
        for (j = 0; j < DEFAULT_GRID_WIDTH; j++) {
            grid[i][j].value = int_grid[i][j];
            grid[i][j].is_const = FALSE;
        }
    }
    write_board_to_file(grid, 9, 9, 3, 3, stdout ,solve_mode);*/
    /*
    FILE *fd = fopen("test.txt","r");
    int a,b,c,d; 
    struct Cell **grid;
    char read_buffer[2500];
    if(fd < 0){
        printf("couldnt read the goddam file \n");
        return -1;
    }
    
    read_board_from_file(fd,&grid,&a,&b,&c,&d);
    
    printf("D ");
    print_board(grid,9,9,3,3,solve_mode,0);
    fflush(stdout);

    

}*/
