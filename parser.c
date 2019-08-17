#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
/*#include "mainaux.h"*/
/*#include "game.h"*/

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


/**
 * Parse a game command from input stream. A command that receives an input and returns a command struct
 *containing the parsed command from the user.
 * @return {Command} The parsed command struct.
 */
command parse_command(){

    char *result;
    char user_input[MAX_COMMAND_LENGTH];
    command received_command;
    char *token;
    char *pos;
    int i = 0;

    /* Reached EOF - Exit! */
    if (feof(stdin)) {
        printf(EXIT_MSG);
        exit(-1);
    }
    /* User input as a whole row. */
    result = fgets(user_input, MAX_COMMAND_LENGTH, stdin);
    /* Check what caused the fgets to fail */
    if (result == NULL) {
        if (feof(stdin)) {
            printf(EXIT_MSG);
            exit(-1);
        }

        else if (ferror(stdin)){
            printf(FUNCTION_FAILED, "fgets");
        }
    }

    /* Skip the empty lines */
    while (strcmp(user_input, "\n") == 0 || strcmp(user_input,"\r\n") == 0) {
        result = fgets(user_input, MAX_COMMAND_LENGTH, stdin);

        /* Check what caused the fgets to fail */
        if (result == NULL) {
            if (feof(stdin)) {
                printf(EXIT_MSG);
                exit(-1);
            }

            else if (ferror(stdin)){
                printf(FUNCTION_FAILED, "fgets");
            }
        }
    }
    /* Split the input */
    token = strtok(user_input, " ");

    /* Remove trailing \n if it exists in the input */
    if ((pos=strchr(token, '\n')) != NULL)
        *pos = '\0';

    /*finished screening for white spaces and EOF, start screening the command*/


    /*Identifying the command entered.*/

    if(strcmp(token,"solve") == 0){
        received_command.command_chosen = solve_command;
        received_command.param_amount = 1;

    } else if(strcmp(token,"edit") == 0){
        received_command.command_chosen = edit_command;
        received_command.param_amount = 1;

    } else if(strcmp(token,"mark_errors") == 0){
        received_command.command_chosen = mark_errors_command;
        received_command.param_amount = 1;

    } else if(strcmp(token,"print_board") == 0){
        received_command.command_chosen = print_board_command;
        received_command.param_amount = 0;

    } else if(strcmp(token,"set") == 0) {
        received_command.command_chosen = set_command;
        received_command.param_amount = 3;

    } else if(strcmp(token,"validate") == 0) {
        received_command.command_chosen = validate_command;
        received_command.param_amount = 0;

    } else if(strcmp(token,"guess") == 0) {
        received_command.command_chosen = guess_command;
        received_command.param_amount = 1;

    } else if(strcmp(token,"generate") == 0) {
        received_command.command_chosen = generate_command;
        received_command.param_amount = 2;

    } else if(strcmp(token,"undo") == 0){
        received_command.command_chosen = undo_command;
        received_command.param_amount = 0;
        
    } else if(strcmp(token,"redo") == 0){
        received_command.command_chosen = redo_command;
        received_command.param_amount = 0;
        
    } else if(strcmp(token,"save") == 0){
        received_command.command_chosen = save_command;
        received_command.param_amount = 1;
        
    } else if(strcmp(token,"hint") == 0){
        received_command.command_chosen = hint_command;
        received_command.param_amount = 2;
        
    } else if(strcmp(token,"guess_hint") == 0){
        received_command.command_chosen = guess_hint_command;
        received_command.param_amount = 2;
        
    } else if(strcmp(token,"num_solutions") == 0){
        received_command.command_chosen = num_solutions_command;
        received_command.param_amount = 0;
        
    }  else if(strcmp(token,"autofill") == 0){
        received_command.command_chosen = autofill_command;
        received_command.param_amount = 0;
        
    }  else if(strcmp(token,"reset") == 0){
        received_command.command_chosen = reset_command;
        received_command.param_amount = 0;
        
    }  else if(strcmp(token,"exit") == 0){
        received_command.command_chosen = exit_command;
        received_command.param_amount = 0;
        
    } else {
        /*invalid command enterted*/
        received_command.command_chosen = invalid_type;
        return received_command;
    }

    /*tokenizing and counting parameters*/
    /*NOTICE: TWO DIGITS, FILE PATH*/
    token = strtok(NULL," ");
    while(token != NULL){
        received_command.param_amount--;

        /*checking if we have not overstepped the amount of parameters*/
        if(received_command.param_amount >= 0){
            /*whether we need to save a path or a parameter*/
            if(received_command.command_chosen == edit_command || 
            received_command.command_chosen == save_command || received_command.command_chosen == solve_command){
                if(token != NULL){
                    /* TODO: Check that the command  strcpy succeeded */
                    strcpy(received_command.path, token);
                } 
                
            } else {

                received_command.params[i]= string_to_int(token) ;
            }
        }

        token = strtok(NULL, " \t\r\n");
        i++;
    }
    /*if param amount is not 0 then we have the wrong amount of parameters.*/

    if(received_command.command_chosen == edit_command && received_command.param_amount == 1){
        received_command.path[0] = '\0';
    }

    return received_command;
}



int write_board_to_file(struct Cell** grid, int grid_height, int grid_width, int box_width, int box_height, FILE *fd, game_mode mode_of_game){

    int i, j, token;
    char temp;

    token = fprintf(fd, "%d %d\n",box_width, box_height);
    if (!token){
                /*ERROR HANDLING FOR FPRINTF*/
                return FAILURE;
        }

    for(i = 0; i < grid_height; i++){
        for(j = 0; j < grid_width; j++){

            if(mode_of_game == edit_mode){
                if(grid[i][j].value == EMPTY){
                    token = fprintf(fd, "%d ",grid[i][j].value);
                } else {
                    token = fprintf(fd, "%d. ",grid[i][j].value);
                }
            } else {
                if(grid[i][j].is_const == TRUE){
                    token = fprintf(fd, "%d. ",grid[i][j].value);

                } else {
                    token = fprintf(fd, "%d ",grid[i][j].value);

                }
            }
            if (!token){
                /*ERROR HANDLING FOR FPRINTF*/
                return FAILURE;
            }
        }
        token = fprintf(fd, "\n",grid[i][j].value);
         if (!token){
            /*ERROR HANDLING FOR FPRINTF*/
            return FAILURE;
        }
    }
    return 1;

}

int read_board_from_file(FILE *fd, struct Cell*** grid_pointer, int *grid_height_pointer, int *grid_width_pointer,
                         int *box_height_pointer,
                         int *box_width_pointer) {
    

    int rows_amount, columns_amount, total_length;
    int i,j,values_read_amount, curr_val;
    int cur_row, cur_col;
    char read_buffer[BUFFER_SIZE];
    char *tok;

    int read_token=0;
    printf("hatool");
    
    /*parsing the file into the board*/
    values_read_amount = 0;

    /*reading as much as we can*/
    while(fread(read_buffer, sizeof(char),BUFFER_SIZE -1,fd) > 0){
        printf("hatool2");
        tok = strtok(read_buffer," \t\r\n");
        values_read_amount++;
        /*parsing while we can*/
        while (tok)
        {
            printf("hatool3");
            if(values_read_amount == 1){
                rows_amount = string_to_int(tok);
                printf("POF");
                *box_height_pointer = rows_amount;
                printf("BOF");
                printf("rows:%d\n",rows_amount);
            } else if(values_read_amount == 2){
                columns_amount = string_to_int(tok);
                printf("cols:%d\n",columns_amount);
                total_length = rows_amount * columns_amount;
                *grid_pointer = create_empty_board(total_length, total_length);

                *box_width_pointer = columns_amount;
            } else {

                *grid_height_pointer = total_length;
                *grid_width_pointer = total_length;
                cur_row = (values_read_amount-3) / total_length;
                if((values_read_amount-2) % total_length == 0){
                    cur_col = total_length -1;
                }
                else{
                    cur_col = (values_read_amount-2) % total_length -1;
                }
                curr_val = (int)strtol(tok,NULL, 10);/**/

                /*checking for legal values*/
                if(curr_val < 0 || curr_val > total_length){
                    /*could not read board-  wrong values inserted*/
                    printf("FAILED! incorrect range! val is:%d \n",curr_val);
                    free(*grid_pointer);
                    return FAILURE;
                }
                printf("%d\n",values_read_amount);
                (*grid_pointer)[cur_row][cur_col].value = curr_val;
                (*grid_pointer)[cur_row][cur_col].is_const = check_if_const(tok);
                (*grid_pointer)[cur_row][cur_col].is_valid = 1;
            }
            tok = strtok(NULL," \t\r\n");
            values_read_amount++;
        }
    }
    
    if(values_read_amount < total_length*total_length){
        /*not enough values were read!*/
        free(*grid_pointer);
        printf("FAILED! not enough params read from file! \n");
        return FAILURE;
    }

    return 1;
    
    


}




/**
 * converts a string to a board
 * 
 * 
 */
void string_to_board(struct Cell** grid, int grid_height, int grid_width, char* board_string ){
    int i,j, k, temp_num;
    char *token = strtok(board_string," \t\r\n");
    for ( i = 0; i < grid_height ; i++ ){
        for( j = 0; j < grid_width; j++){
            if(token[strlen(token)-1]=='.'){

                grid[i][j].is_const=TRUE;
                printf("!");
            }
            grid[i][j].value=string_to_int(token);
            printf("%s,",token);
            token = strtok(NULL, " \t\r\n");
        }
        printf("\n");
    }


}

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