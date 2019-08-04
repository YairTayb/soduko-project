#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "mainaux.h"
#include "game.h"

int string_to_int(char *number ){
    int i, num;
    i = 0;
    num = 0;
    /*check length before*/
    while(number[i] != '.' && number[i] ){
        num = num * 10;
        num += number[i] -'0';
        i++;
    }
    return num;


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
        printf("AA1\n");
        received_command.command_chosen = solve_command;
        received_command.param_amount = 1;

    } else if(strcmp(token,"edit") == 0){
        printf("AA2\n");
        received_command.command_chosen = edit_command;
        received_command.param_amount = 1;

    } else if(strcmp(token,"mark_errors") == 0){
        received_command.command_chosen = mark_errors_command;
        received_command.param_amount = 1;

    } else if(strcmp(token,"print_board") == 0){
        received_command.command_chosen = print_board_command;
        received_command.param_amount = 0;

    } else if(strcmp(token,"set") == 0) {
        printf("AA4\n");
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

                strcpy(received_command.path, token);
            } else {

                received_command.params[i]= string_to_int(token) ;
            }
        }

        token = strtok(NULL, " \t\r\n");
        i++;
    }
    /*if param amount is not 0 then we have the wrong amount of parameters.*/

    return received_command;
}



void write_board_to_file(struct Cell** grid, int grid_height, int grid_width, int box_width, int box_height, FILE *fd, game_mode mode_of_game){

    int i, j, token;
    char temp;

    token = fprintf(fd, "%d %d\n",box_width, box_height);
    if (!token){
                /*ERROR HANDLING FOR FPRINTF*/
                exit(-1);
        }

    for(i = 0; i < grid_height; i++){
        for(j = 0; j < grid_width; j++){

            if(mode_of_game == edit){
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
                exit(-1);
            }
        }
        token = fprintf(fd, "\n",grid[i][j].value);
         if (!token){
            /*ERROR HANDLING FOR FPRINTF*/
            exit(-1);
        }
    }

}


/*
char** board_to_string(struct Cell** grid, int grid_height, int grid_width, int fd){

    int i, j;
    char ** parsed_board = (char **)(malloc(sizeof(char **)*grid_height));
    for (i = 0; i < grid_height; i++){
        parsed_board[i] = (char *)(malloc((sizeof(char) * 4) + 2));
    }
    

}*/
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


int main(){
   /* int i;
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

    int int_grid[9][9] = {{4, 0, 1, 0, 0, 0, 0, 5, 2},
                          {0, 0, 0, 0, 0, 0, 0, 0, 0},
                          {0, 5, 0, 0, 4, 1, 0, 0, 0},
                          {0, 0, 0, 2, 0, 0, 5, 4, 0},
                          {0, 1, 0, 4, 0, 0, 0, 2, 3},
                          {0, 4, 2, 0, 0, 0, 1, 0, 0},
                          {0, 0, 4, 0, 0, 0, 0, 0, 0},
                          {0, 7, 0, 1, 0, 0, 0, 0, 4},
                          {0, 0, 0, 7, 0, 0, 0, 0, 0}};
    struct Cell **grid = (struct Cell **) malloc(GRID_HEIGHT * sizeof(struct Cell));
    int i;
    int j;
    /*check if malloc failed*/
    if(!grid){
        printf(FUNCTION_FAILED, "malloc");
        exit(0);
    }

    for (i = 0; i < GRID_HEIGHT; i++){
        grid[i] = (struct Cell *) malloc(GRID_WIDTH * sizeof(struct Cell));
        /*check if malloc failed*/
        if(!grid[i]){
            printf(FUNCTION_FAILED, "malloc");
            exit(0);
        }

    }

    /* Initiate the board */
    for (i = 0; i < GRID_HEIGHT; i++) {
        for (j = 0; j < GRID_WIDTH; j++) {
            grid[i][j].value = int_grid[i][j];
            grid[i][j].is_const = FALSE;
        }
    }
    write_board_to_file(grid, 9, 9, 3, 3, stdout ,solve);






}