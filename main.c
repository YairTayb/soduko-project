/*created by Yair on 3.6.19*/
#include "parser.h"
#include "game.h"
#include "mainaux.h"
#include "solver.h"

#include <stdlib.h>
#include <stdio.h>


int main(int argc, char *argv[]) {
    struct Cell **board;
    int i;
    int grid_height, grid_width, box_height, box_width;
    command user_input;

    /* Game begins */
    while (TRUE) {
        /* Parse the current command */
        user_input = parse_command();

        if (user_input.command_chosen == solve_command){
            /* Perform solve command */
            solve(&board, user_input.path, &grid_height, &grid_width, &box_height, &box_width);
        }
        else if {
            edit(&board, user_input.path, user_input.params[0], &grid_height, &grid_width, &box_height, &box_width);

        }

    }

}