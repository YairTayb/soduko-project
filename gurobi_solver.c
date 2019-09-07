#include <string.h>
#include <stdlib.h>

#include "gurobi_solver.h"
#include "consts.h"
#include "solver.h"
#include "mainaux.h"
#include "gurobi_c.h"

returnCodeDesc add_variables_to_model(GRBenv *env, GRBmodel * model, int total_num_of_variables,
        linear_solving_mode solving_mode, int dim) {
    returnCodeDesc return_code_desc;

    int error = 0;
    int i = 0;
    double *obj;
    char *vtype;

    obj = malloc(sizeof(double) * total_num_of_variables);

    if (obj == NULL) {
        /* TODO: Maybe we should just exit? */
        return_code_desc.error_code = E_FUNCTION_FAILED;
        sprintf(return_code_desc.error_message, FUNCTION_FAILED, "malloc");
        return return_code_desc;
    }

    vtype = malloc(sizeof(char) * total_num_of_variables);

    if (vtype == NULL) {
        /* TODO: Maybe we should just exit? */
        free(obj);
        return_code_desc.error_code = E_FUNCTION_FAILED;
        sprintf(return_code_desc.error_message, FUNCTION_FAILED, "malloc");
        return return_code_desc;
    }

    /* Set the coefficients of the objective and the type of the variables in the model */
    for (i = 0; i < total_num_of_variables; i++) {
        if (solving_mode == ILP) {
            obj[i] = 0; /* Set the coefficients to 0 for all variables when running ILP */
            vtype[i] = GRB_BINARY;
        } else {
            /* Set coefficients to random to produce good objective when running LP */
            obj[i] = (rand() % (dim * dim)) + 1;
            vtype[i] = GRB_CONTINUOUS;
        }
    }

    /* Add the variables to the model. The variables of the model are Xijk, which stands for cell <i,j> with value k.
     * The model (depending on the solving mode) will calculate the possibility of value k to be in cell <i,j> and set
     * the solution to variable Xijk.
     * If an error occurrs, error will be set to non-zero value.*/
    if ((error = GRBaddvars(model, total_num_of_variables, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype, NULL)) != 0) {
        /* TODO: Handle errors */
        return_code_desc.error_code = E_GUROBI_FAILURE;
        sprintf(return_code_desc.error_message, "Error: %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
    }

    /* Change objective sense to maximization */
    else if ((error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE)) != 0) {
        return_code_desc.error_code = E_GUROBI_FAILURE;
        sprintf(return_code_desc.error_message, "Error: %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
    }

    /* Update the model */
    else if ((error = GRBupdatemodel(model)) != 0) {
        return_code_desc.error_code = E_GUROBI_FAILURE;
        sprintf(return_code_desc.error_message, "Error: %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
    }

    else {
        return_code_desc.error_code = E_SUCCESS;
        strcpy(return_code_desc.error_message, NO_ERRORS);
    }

    free(obj);
    free(vtype);

    return return_code_desc;

}

returnCodeDesc
add_cell_single_value_constraints(GRBenv *env, GRBmodel *model, int* indices_array, linear_solving_mode solving_mode,
                     int grid_height, int grid_width, int box_height, int box_width, int row,
                     int col) {
    returnCodeDesc return_code_desc;
    int* ind = NULL;
    double* val = NULL;

    int error = 0;
    int value;
    int i = 0;
    int index = grid_height;
    int num_of_valid_values = 0;
    double coefficient;

    /* Calculate the number of valid values for the current cell */
    for (value = 0; value < (box_height * box_width); value++) {
        if (indices_array[MULTIDIM_ARR_LOC(row, col, value, grid_height, grid_width, (box_height * box_width))] > -1) {
            num_of_valid_values++;
        }
    }

    if (num_of_valid_values > 0) {
        /* The ind array is a mapping of the variables in regarding of the objective function.
             * For example if the objective is max x+y+z, and the constraint is x+z<3, then the ind array
             * should be [0,2]. */
        ind = malloc(sizeof(int) * num_of_valid_values);

        if (ind == NULL) {
            return_code_desc.error_code = E_FUNCTION_FAILED;
            sprintf(return_code_desc.error_message, FUNCTION_FAILED, "malloc");
            return return_code_desc;
        }

        /* The val array holds the coefficients of the matching variable in the ind array in the current constraint.
         * For example, if constraint is x+z<3, and ind is [0,2] then val should be [1,1] */
        val = malloc(sizeof(double) * num_of_valid_values);

        if (val == NULL) {
            return_code_desc.error_code = E_FUNCTION_FAILED;
            sprintf(return_code_desc.error_message, FUNCTION_FAILED, "malloc");
            free(ind);
            return return_code_desc;
        }

        /* Find the valid values for the given cell at the given board */
        for (value = 0; value < (box_height * box_width); value++) {
            /* For variable Xijk (cell <i,j> with value of k) set constraints so the sum of all variables for cell <i,j>
            * will be 1 (meaning if the cell <i,j> has 3 valid values equals to 1, 2, 3 then Xij1+Xij2+Xij3 = 1) */
            index = indices_array[MULTIDIM_ARR_LOC(row, col, value, grid_height, grid_width, (box_height * box_width))];
            if (index >= 0) {
                if (solving_mode == LP) {
                    /* If the solving mode is LP - then for each Xijk (cell <i,j> with value of k) variable, add a constraint
                     * to be between 0.0 and 1.0 */
                    coefficient = 1;

                    /* Add the upper bound constraint for Xijk <= 1 in LP mode */
                    if ((error = GRBaddconstr(model, 1, &index, &coefficient, GRB_LESS_EQUAL, 1.0, NULL)) != 0) {
                        return_code_desc.error_code = E_GUROBI_FAILURE;
                        sprintf(return_code_desc.error_message, "Error: %d GRBaddconstr(): %s\n", error,
                                GRBgeterrormsg(env));
                        free(ind);
                        free(val);
                        return return_code_desc;
                    }

                        /* Add the lower bound constraint for Xijk >= 0 in LP mode */
                    else if ((error = GRBaddconstr(model, 1, &index, &coefficient, GRB_GREATER_EQUAL, 0.0, NULL)) !=
                             0) {
                        return_code_desc.error_code = E_GUROBI_FAILURE;
                        sprintf(return_code_desc.error_message, "Error: %d GRBaddconstr(): %s\n", error,
                                GRBgeterrormsg(env));
                        free(ind);
                        free(val);
                        return return_code_desc;
                    }
                }

                ind[i] = index;
                val[i] = 1;
                i++;
            }
        }

        /* Add the sum upper constraint for the variables Xij1 + ... + Xijk <= 1 (k = dimension of the board) */
        if ((error = GRBaddconstr(model, num_of_valid_values, ind, val, GRB_LESS_EQUAL, 1.0, NULL)) != 0) {
            return_code_desc.error_code = E_GUROBI_FAILURE;
            sprintf(return_code_desc.error_message, "Error: %d GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
            free(ind);
            free(val);
            return return_code_desc;
        }

        /* Add the sum lower constraint for the variables Xij1 + ... + Xijk >= 1 (k = dimension of the board) */
        else if ((error = GRBaddconstr(model, num_of_valid_values, ind, val, GRB_GREATER_EQUAL, 1.0, NULL)) != 0) {
            return_code_desc.error_code = E_GUROBI_FAILURE;
            sprintf(return_code_desc.error_message, "Error: %d GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
            free(ind);
            free(val);
            return return_code_desc;
        }

        free(ind);
        free(val);
    }

    return_code_desc.error_code = E_SUCCESS;
    strcpy(return_code_desc.error_message, NO_ERRORS);
    return return_code_desc;

}

returnCodeDesc
add_value_single_row_constraint(GRBenv *env, GRBmodel *model, int* indices_array,
                                int grid_height, int grid_width, int box_height, int box_width, board game_board,
                                int row, int k_value) {
    returnCodeDesc return_code_desc;
    int* ind = NULL;
    double* val = NULL;

    int error = 0;
    int index;
    int col = 0;
    int num_of_valid_cells = 0;

    /* The ind array is a mapping of the variables in regarding of the objective function.
     * For example if the objective is max x+y+z, and the constraint is x+z<3, then the ind array
     * should be [0,2]. */
    ind = malloc(sizeof(int) * (box_height * box_width));

    if (ind == NULL) {
        return_code_desc.error_code = E_FUNCTION_FAILED;
        sprintf(return_code_desc.error_message, FUNCTION_FAILED, "malloc");
        return return_code_desc;
    }

    /* The val array holds the coefficients of the matching variable in the ind array in the current constraint.
     * For example, if constraint is x+z<3, and ind is [0,2] then val should be [1,1] */
    val = malloc(sizeof(double) * (box_height * box_width));

    if (val == NULL) {
        return_code_desc.error_code = E_FUNCTION_FAILED;
        sprintf(return_code_desc.error_message, FUNCTION_FAILED, "malloc");
        free(ind);
        return return_code_desc;
    }

    /* Find the cells in the given row that k_value is a valid value for them, and add them to the constraint.
     * The constraint is the following: Xi1k + Xi2k + ... + Xijk = 1, while i = given row and k = given k_value.
     * In other words, the sum of variables in the given row for the value k_value, should sum 1.
     * This will insure the number k_value will appear once (or will be summed to 1 in LP) in a row. */
    for (col = 0; col < grid_width; col++) {
        /* According to the instructions, a variable Xijk should be added to the model only if k is valid value for
         * cell <i,j>. So verify k_value is actually valid for cell <row, col> and if it is, add it to the constraint.
         * If its not - then the variable is not part of the model and should be ignored. */
        if (game_board[row][col].is_const == FALSE && game_board[row][col].value == UNASSIGNED) {
            if (is_valid(game_board, grid_height, grid_width, box_height, box_width, row, col, k_value)) {
                index = indices_array[
                        MULTIDIM_ARR_LOC(row, col, k_value - 1, grid_height, grid_width, (box_height * box_width))];
                if (index >= 0) {
                    ind[num_of_valid_cells] = index;
                    val[num_of_valid_cells] = 1;
                    num_of_valid_cells++;
                }

            }
        }
    }

    if (num_of_valid_cells > 0) {
        /* Add the sum upper constraint for the variables Xi1k + ... + Xijk <= 1 (j = dimension of the board) */
        if ((error = GRBaddconstr(model, num_of_valid_cells, ind, val, GRB_LESS_EQUAL, 1.0, NULL)) != 0) {
            return_code_desc.error_code = E_GUROBI_FAILURE;
            sprintf(return_code_desc.error_message, "Error: %d GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
            free(ind);
            free(val);
            return return_code_desc;
        }

            /* Add the sum lower constraint for the variables Xi1k + ... + Xijk <= 1 (j = dimension of the board) */
        else if ((error = GRBaddconstr(model, num_of_valid_cells, ind, val, GRB_GREATER_EQUAL, 1.0, NULL)) != 0) {
            return_code_desc.error_code = E_GUROBI_FAILURE;
            sprintf(return_code_desc.error_message, "Error: %d GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
            free(ind);
            free(val);
            return return_code_desc;
        }
    }

    free(ind);
    free(val);

    return_code_desc.error_code = E_SUCCESS;
    strcpy(return_code_desc.error_message, NO_ERRORS);
    return return_code_desc;

}

returnCodeDesc add_value_row_constraints(GRBenv *env, GRBmodel *model, int* indices_array,
                                         int grid_height, int grid_width, int box_height, int box_width,
                                         board game_board, int k_value) {
    returnCodeDesc return_code_desc;
    int row;

    for (row = 0; row < grid_height; row++) {
        /* Add the value constraint for the current row */
        return_code_desc = add_value_single_row_constraint(env, model, indices_array,
                grid_height, grid_width, box_height, box_width, game_board, row, k_value);

        if (is_error(return_code_desc) == TRUE) {
            return return_code_desc;
        }
    }

    return_code_desc.error_code = E_SUCCESS;
    strcpy(return_code_desc.error_message, NO_ERRORS);
    return return_code_desc;
}

returnCodeDesc
add_value_col_single_constraint(GRBenv *env, GRBmodel *model, int* indices_array,
                                int grid_height, int grid_width, int box_height, int box_width, board game_board,
                                int col, int k_value) {
    returnCodeDesc return_code_desc;
    int* ind = NULL;
    double* val = NULL;

    int error = 0;
    int index;
    int row = 0;
    int num_of_valid_cells = 0;

    /* The ind array is a mapping of the variables in regarding of the objective function.
     * For example if the objective is max x+y+z, and the constraint is x+z<3, then the ind array
     * should be [0,2]. */
    ind = malloc(sizeof(int) * (box_height * box_width));

    if (ind == NULL) {
        return_code_desc.error_code = E_FUNCTION_FAILED;
        sprintf(return_code_desc.error_message, FUNCTION_FAILED, "malloc");
        return return_code_desc;
    }

    /* The val array holds the coefficients of the matching variable in the ind array in the current constraint.
     * For example, if constraint is x+z<3, and ind is [0,2] then val should be [1,1] */
    val = malloc(sizeof(double) * (box_height * box_width));

    if (val == NULL) {
        return_code_desc.error_code = E_FUNCTION_FAILED;
        sprintf(return_code_desc.error_message, FUNCTION_FAILED, "malloc");
        free(ind);
        return return_code_desc;
    }

    /* Find the cells in the given col that k_value is a valid value for them, and add them to the constraint.
     * The constraint is the following: X1jk + X2jk + ... + Xijk = 1, while j = given col and k = given k_value.
     * In other words, the sum of variables in the given col for the value k_value, should sum 1.
     * This will insure the number k_value will appear once (or will be summed to 1 in LP) in a col. */
    for (row = 0; row < grid_height; row++) {
        /* According to the instructions, a variable Xijk should be added to the model only if k is valid value for
         * cell <i,j>. So verify k_value is actually valid for cell <row, col> and if it is, add it to the constraint.
         * If its not - then the variable is not part of the model and should be ignored. */
        if (game_board[row][col].is_const == FALSE && game_board[row][col].value == UNASSIGNED) {
            if (is_valid(game_board, grid_height, grid_width, box_height, box_width, row, col, k_value)) {
                index = indices_array[
                        MULTIDIM_ARR_LOC(row, col, k_value - 1, grid_height, grid_width, (box_height * box_width))];
                if (index >= 0) {
                    ind[num_of_valid_cells] = index;
                    val[num_of_valid_cells] = 1;
                    num_of_valid_cells++;
                }
            }
        }
    }

    if (num_of_valid_cells > 0) {

        /* Add the sum upper constraint for the variables X1jk + X2jk + ... + Xijk <= 1 (j = dimension of the board) */
        if ((error = GRBaddconstr(model, num_of_valid_cells, ind, val, GRB_LESS_EQUAL, 1.0, NULL)) != 0) {
            return_code_desc.error_code = E_GUROBI_FAILURE;
            sprintf(return_code_desc.error_message, "Error: %d GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
            free(ind);
            free(val);
            return return_code_desc;
        }

            /* Add the sum lower constraint for the variables X1jk + X2jk + ... + Xijk <= 1 (j = dimension of the board) */
        else if ((error = GRBaddconstr(model, num_of_valid_cells, ind, val, GRB_GREATER_EQUAL, 1.0, NULL)) != 0) {
            return_code_desc.error_code = E_GUROBI_FAILURE;
            sprintf(return_code_desc.error_message, "Error: %d GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
            free(ind);
            free(val);
            return return_code_desc;

        }
    }

    free(ind);
    free(val);

    return_code_desc.error_code = E_SUCCESS;
    strcpy(return_code_desc.error_message, NO_ERRORS);
    return return_code_desc;

}

returnCodeDesc add_value_col_constraints(GRBenv *env, GRBmodel *model, int* indices_array,
                                         int grid_height, int grid_width, int box_height, int box_width,
                                         board game_board, int k_value) {
    returnCodeDesc return_code_desc;
    int col;

    for (col = 0; col < grid_width; col++) {
        /* Add the value constraint for the current col */
        return_code_desc = add_value_col_single_constraint(env, model, indices_array,
                                                           grid_height, grid_width, box_height, box_width, game_board, col, k_value);

        if (is_error(return_code_desc) == TRUE) {
            return return_code_desc;
        }
    }

    return_code_desc.error_code = E_SUCCESS;
    strcpy(return_code_desc.error_message, NO_ERRORS);
    return return_code_desc;
}

returnCodeDesc
add_value_single_box_constraint(GRBenv *env, GRBmodel *model, int* indices_array,
                                int grid_height, int grid_width, int box_height, int box_width, board game_board,
                                int box_start_row, int box_start_col, int k_value) {
    returnCodeDesc return_code_desc;
    int* ind = NULL;
    double* val = NULL;

    int error = 0;
    int index;
    int row;
    int col;
    int num_of_valid_cells = 0;

    /* The ind array is a mapping of the variables in regarding of the objective function.
     * For example if the objective is max x+y+z, and the constraint is x+z<3, then the ind array
     * should be [0,2]. */
    ind = malloc(sizeof(int) * (box_height * box_width));

    if (ind == NULL) {
        return_code_desc.error_code = E_FUNCTION_FAILED;
        sprintf(return_code_desc.error_message, FUNCTION_FAILED, "malloc");
        return return_code_desc;
    }

    /* The val array holds the coefficients of the matching variable in the ind array in the current constraint.
     * For example, if constraint is x+z<3, and ind is [0,2] then val should be [1,1] */
    val = malloc(sizeof(double) * (box_height * box_width));

    if (val == NULL) {
        return_code_desc.error_code = E_FUNCTION_FAILED;
        sprintf(return_code_desc.error_message, FUNCTION_FAILED, "malloc");
        free(ind);
        return return_code_desc;
    }

    /* Find the cells in the given box that k_value is a valid value for them, and add them to the constraint.
     * The constraint is the following: the sum of variables in the given box for the value k_value, should sum 1.
     * This will insure the number k_value will appear once (or will be summed to 1 in LP) in a box. */
    for (row = box_start_row; row < (box_start_row + box_height); row++) {
        for (col = box_start_col; col < (box_start_col + box_width); col++) {
            /* According to the instructions, a variable Xijk should be added to the model only if k is valid value for
             * cell <i,j>. So verify k_value is actually valid for cell <row, col> and if it is, add it to the constraint.
             * If its not - then the variable is not part of the model and should be ignored. */
            if (game_board[row][col].is_const == FALSE && game_board[row][col].value == UNASSIGNED) {
                if (is_valid(game_board, grid_height, grid_width, box_height, box_width, row, col, k_value)) {
                    index = indices_array[
                            MULTIDIM_ARR_LOC(row, col, k_value - 1, grid_height, grid_width, (box_height * box_width))];

                    if (index >= 0) {
                        ind[num_of_valid_cells] = index;
                        val[num_of_valid_cells] = 1;
                        num_of_valid_cells++;
                    }
                }
            }
        }
    }

    if (num_of_valid_cells > 0) {

        /* Add the sum upper constraint for the variables sum in the box <= 1 */
        if ((error = GRBaddconstr(model, num_of_valid_cells, ind, val, GRB_LESS_EQUAL, 1.0, NULL)) != 0) {
            return_code_desc.error_code = E_GUROBI_FAILURE;
            sprintf(return_code_desc.error_message, "Error: %d GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
            free(ind);
            free(val);
            return return_code_desc;
        }

            /* Add the sum lower constraint for the variables sum in the box <= 1 */
        else if ((error = GRBaddconstr(model, num_of_valid_cells, ind, val, GRB_GREATER_EQUAL, 1.0, NULL)) != 0) {
            return_code_desc.error_code = E_GUROBI_FAILURE;
            sprintf(return_code_desc.error_message, "Error: %d GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
            free(ind);
            free(val);
            return return_code_desc;
        }
    }

    free(ind);
    free(val);

    return_code_desc.error_code = E_SUCCESS;
    strcpy(return_code_desc.error_message, NO_ERRORS);
    return return_code_desc;

}

returnCodeDesc add_value_box_constraints(GRBenv *env, GRBmodel *model, int* indices_array,
                                         int grid_height, int grid_width, int box_height, int box_width,
                                         board game_board, int k_value) {
    returnCodeDesc return_code_desc;
    int box_start_row = 0;
    int box_start_col = 0;

    for (box_start_row = 0; box_start_row < grid_height; box_start_row += box_height) {
        for (box_start_col = 0; box_start_col < grid_width; box_start_col += box_width) {
            /* Add the value constraint for the current box */
            return_code_desc = add_value_single_box_constraint(env, model, indices_array,
                                                               grid_height, grid_width, box_height, box_width,
                                                               game_board, box_start_row, box_start_col, k_value);

            if (is_error(return_code_desc) == TRUE) {
                return return_code_desc;
            }
        }
    }

    return_code_desc.error_code = E_SUCCESS;
    strcpy(return_code_desc.error_message, NO_ERRORS);
    return return_code_desc;
}

returnCodeDesc add_constraints(GRBenv *env, GRBmodel *model, int* indices_array,
                               linear_solving_mode solving_mode,
                               int grid_height, int grid_width, int box_height, int box_width,
                               board game_board) {
    returnCodeDesc return_code_desc;
    int k_value;
    int row, col;

    for (row = 0; row < grid_height; row++) {
        for (col = 0; col < grid_width; col++) {
            if (game_board[row][col].is_const == FALSE && game_board[row][col].value == UNASSIGNED) {
                /* For each empty cell set its own constrains - no cell can hold more than one value */
                return_code_desc = add_cell_single_value_constraints(env, model, indices_array, solving_mode, grid_height, grid_width,
                                                                     box_height, box_width, row, col);

                if (is_error(return_code_desc) == TRUE) {
                    return return_code_desc;
                }
            }
        }
    }

    /* Add all the constraints for a possible value (1... dim of the board). Constraints are:
     * - Every value from 1 to dim must appear exactly once in every row.
     * - Every value from 1 to dim must appear exactly once in every col.
     * - Every value from 1 to dim must appear exactly once in every box.
     * */
    for (k_value = 1; k_value <= (box_height * box_width); k_value++){
        /* Add row constrains */
        return_code_desc = add_value_row_constraints(env, model, indices_array, grid_height,
                grid_width, box_height, box_width, game_board, k_value);

        if (is_error(return_code_desc) == TRUE) {
            return return_code_desc;
        }

        /* Add col constrains */
        return_code_desc = add_value_col_constraints(env, model, indices_array, grid_height,
                                                     grid_width, box_height, box_width, game_board, k_value);

        if (is_error(return_code_desc) == TRUE) {
            return return_code_desc;
        }

        /* Add box constrains */
        return_code_desc = add_value_box_constraints(env, model, indices_array, grid_height,
                                                     grid_width, box_height, box_width, game_board, k_value);

        if (is_error(return_code_desc) == TRUE) {
            return return_code_desc;
        }

    }

    return_code_desc.error_code = E_SUCCESS;
    strcpy(return_code_desc.error_message, NO_ERRORS);
    return return_code_desc;

}

returnCodeDesc solve_model(GRBenv *env, GRBmodel *model) {
    returnCodeDesc return_code_desc;
    int error = 0;
    int optimstatus;

    if ((error = GRBoptimize(model)) != 0) {
        return_code_desc.error_code = E_GUROBI_FAILURE;
        sprintf(return_code_desc.error_message, "Error: %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
        return return_code_desc;
    }

    if ((error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus)) != 0){
        return_code_desc.error_code = E_GUROBI_FAILURE;
        sprintf(return_code_desc.error_message, "Error: %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
        return return_code_desc;
    }

    if (optimstatus == GRB_OPTIMAL) {
        /* Solving was successful */
        return_code_desc.error_code = E_SUCCESS;
        strcpy(return_code_desc.error_message, NO_ERRORS);
        return return_code_desc;
    }

    /* No solution was found */
    else if (optimstatus == GRB_INF_OR_UNBD || optimstatus == GRB_INFEASIBLE || optimstatus == GRB_UNBOUNDED) {
        return_code_desc.error_code = E_NO_SOLUTION;
        strcpy(return_code_desc.error_message, VALIDATION_FAILED);
        return return_code_desc;
    }

    /* Some error has occurred or calculation stopped for some reason*/
    else {
        return_code_desc.error_code = E_GUROBI_FAILURE;
        sprintf(return_code_desc.error_message, "Error: %d error has occurred while running Gurobi. Optimstatus: %d. Error: %s\n", error, optimstatus, GRBgeterrormsg(env));
        return return_code_desc;
    }

}


void apply_ILP_solution(double *sol, int* indices_array, board game_board, int grid_height, int grid_width,
                        int box_height, int box_width) {
    int row, col, value, index;

    /* For each empty cell in the board, apply the solution of the ILP */
    for (row = 0; row < grid_height; row++) {
        for (col = 0; col < grid_width; col++) {
            if (game_board[row][col].is_const == FALSE && game_board[row][col].value == UNASSIGNED) {

                /* For each cell <i,j>, check for each value k (1...dim) if Xijk == 1.0. If its true, then the solution
                 * of the ILP has set the cell <i,j> the value k */
                for (value = 0; value < (box_height * box_width); value++) {
                    index = indices_array[MULTIDIM_ARR_LOC(row, col, value, grid_height, grid_width, (box_height * box_width))];

                    if (index > -1) {
                        if (sol[index] == 1.0) {
                            game_board[row][col].value = value + 1;
                            game_board[row][col].has_changed = TRUE;
                            break;
                        }
                    }
                }
            }
        }
    }
}

void apply_LP_solution(double *sol, int* indices_array, board game_board, int grid_height, int grid_width,
                        int box_height, int box_width, double* guess_scores) {
    int row, col, value, index;

    /* For each empty cell in the board, apply the solution of the ILP */
    for (row = 0; row < grid_height; row++) {
        for (col = 0; col < grid_width; col++) {
            if (game_board[row][col].is_const == FALSE && game_board[row][col].value == UNASSIGNED) {

                for (value = 0; value < (box_height * box_width); value++) {
                    guess_scores[MULTIDIM_ARR_LOC(row, col, value, grid_height, grid_width, (box_height * box_width))] = 0;

                    index = indices_array[MULTIDIM_ARR_LOC(row, col, value, grid_height, grid_width, (box_height * box_width))];

                    if (index > -1) {
                        guess_scores[MULTIDIM_ARR_LOC(row, col, value, grid_height, grid_width, (box_height * box_width))] = sol[index];
                    }
                }
            }
        }
    }
}

returnCodeDesc get_solution_and_apply(GRBenv *env, GRBmodel *model, int* indices_array, int total_num_of_variables,
                                          int grid_height, int grid_width, int box_height, int box_width,
                                          board game_board, linear_solving_mode solving_mode, double* guess_scores) {
    returnCodeDesc return_code_desc;
    int error = 0;

    double* sol;

    /* sol array will hold the values of the ILP solution */
    sol = (double *) malloc(sizeof(double) * total_num_of_variables);

    if (sol == NULL) {
        return_code_desc.error_code = E_FUNCTION_FAILED;
        sprintf(return_code_desc.error_message, FUNCTION_FAILED, "malloc");
        return return_code_desc;
    }

    /* Get the solution */
    if ((error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, total_num_of_variables, sol)) != 0) {
        return_code_desc.error_code = E_GUROBI_FAILURE;
        sprintf(return_code_desc.error_message, "Error: %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
        free(sol);
        return return_code_desc;
    }


    if (solving_mode == ILP) {
        /* Apply the solution on to the board */
        apply_ILP_solution(sol, indices_array, game_board, grid_height, grid_width, box_height, box_width);
    }

    else {
        apply_LP_solution(sol, indices_array, game_board, grid_height, grid_width, box_height, box_width, guess_scores);
    }

    free(sol);
    return_code_desc.error_code = E_SUCCESS;
    strcpy(return_code_desc.error_message, NO_ERRORS);
    return return_code_desc;

}

returnCodeDesc num_of_variables(int *variables_indices, int *total_num_of_variables, board game_board, int grid_height, int grid_width,
                                int box_height, int box_width) {
    returnCodeDesc return_code_desc;
    int row, col, value;
    int has_valid_value;
    int total_counter = 0;

    for (row = 0; row < grid_height; row++) {
        for (col = 0; col < grid_width; col++) {
            if (game_board[row][col].is_const == FALSE && game_board[row][col].value == UNASSIGNED) {
                has_valid_value = FALSE;

                for (value = 0; value < (box_height * box_width); value++) {
                    if (is_valid(game_board, grid_height, grid_width, box_height, box_width, row, col, value + 1)) {
                        variables_indices[MULTIDIM_ARR_LOC(row, col, value, grid_height, grid_width,
                                                          (box_height * box_width))] = total_counter;
                        total_counter++;
                        has_valid_value = TRUE;
                    } else {
                        variables_indices[MULTIDIM_ARR_LOC(row, col, value, grid_height, grid_width,
                                                          (box_height * box_width))] = -1;
                    }

                }

                if (has_valid_value == FALSE) {
                    /* There is at least one variable with no valid values at all - board is unsolvable */
                    return_code_desc.error_code = E_NO_SOLUTION;
                    strcpy(return_code_desc.error_message, VALIDATION_FAILED);
                    return return_code_desc;
                }
            }
        }
    }

    *total_num_of_variables = total_counter;

    return_code_desc.error_code = E_SUCCESS;
    strcpy(return_code_desc.error_message, NO_ERRORS);
    return return_code_desc;

}

returnCodeDesc initialize_GRB(GRBenv **env, GRBmodel **model, int *indices_array, int *total_variables_num, board game_board, int grid_height, int grid_width,
                              int box_height, int box_width) {
    returnCodeDesc return_code_desc;
    int error;

    return_code_desc = num_of_variables(indices_array, total_variables_num, game_board, grid_height, grid_width, box_height, box_width);

    if (is_error(return_code_desc) == TRUE) {
        /* At least 1 cell has no possible valid value - board is unsolvable. Raise the error */
        return return_code_desc;
    }

    /* Create a new Gurobi environment */
    if ((error = GRBloadenv(env, NULL)) != 0) {
        return_code_desc.error_code = E_GUROBI_FAILURE;
        sprintf(return_code_desc.error_message, "ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(*env));
        return return_code_desc;
    }

    /* Silence the Gurobi console */
    if ((error = GRBsetintparam(*env, GRB_INT_PAR_LOGTOCONSOLE, 0)) != 0) {
        return_code_desc.error_code = E_GUROBI_FAILURE;
        sprintf(return_code_desc.error_message, "ERROR %d GRBsetintparam(): %s\n", error, GRBgeterrormsg(*env));
        GRBfreeenv(*env);
        return return_code_desc;
    }

    /* Create a new Gurobi model */
    if ((error = GRBnewmodel(*env, model, NULL, 0, NULL, NULL, NULL, NULL, NULL)) != 0) {
        return_code_desc.error_code = E_GUROBI_FAILURE;
        sprintf(return_code_desc.error_message, "ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(*env));
        GRBfreeenv(*env);
        return return_code_desc;
    }

    return_code_desc.error_code = E_SUCCESS;
    strcpy(return_code_desc.error_message, NO_ERRORS);
    return return_code_desc;
}


returnCodeDesc solve_gurobi(board game_board, int grid_height, int grid_width, int box_height, int box_width,
        linear_solving_mode solving_mode, double* guess_scores) {
    returnCodeDesc return_code_desc;

    GRBenv *env = NULL;
    GRBmodel *model = NULL;
    int *indices_array = NULL;

    int total_variables_num = 0;
    int dim = (box_height * box_width);

    /* The indices array holds the mapping for each <row, col, value> to its matching variable index in the linear
     * program's objective and constraints. */
    indices_array = (int *)malloc(grid_height*grid_width*dim*sizeof(int));

    if (indices_array == NULL) {
        return_code_desc.error_code = E_FUNCTION_FAILED;
        sprintf(return_code_desc.error_message, FUNCTION_FAILED, "malloc");
        return return_code_desc;
    }

    return_code_desc = initialize_GRB(&env, &model, indices_array, &total_variables_num, game_board, grid_height, grid_width, box_height, box_width);

    /* Validate initialization */
    if (is_error(return_code_desc) == TRUE) {
        free(indices_array);
        return return_code_desc;
    }

    return_code_desc = add_variables_to_model(env, model, total_variables_num, solving_mode, dim);

    /* Validate objective construction */
    if (is_error(return_code_desc) == TRUE) {
        free(indices_array);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return return_code_desc;
    }

    return_code_desc = add_constraints(env, model, indices_array, solving_mode, grid_height, grid_width, box_height,
                                       box_width, game_board);

    /* Validate constraints adding */
    if (is_error(return_code_desc) == TRUE) {
        free(indices_array);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return return_code_desc;
    }

    return_code_desc = solve_model(env, model);

    /* Validate solving of the model */
    if (is_error(return_code_desc) == TRUE) {
        free(indices_array);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return return_code_desc;
    }

    return_code_desc = get_solution_and_apply(env, model, indices_array, total_variables_num, grid_height, grid_width, box_height,
                                                  box_width, game_board, solving_mode, guess_scores);

    /* Validate applying of the solution */
    if (is_error(return_code_desc) == TRUE) {
        free(indices_array);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return return_code_desc;
    }

    free(indices_array);
    GRBfreemodel(model);
    GRBfreeenv(env);

    return_code_desc.error_code = E_SUCCESS;
    strcpy(return_code_desc.error_message, NO_ERRORS);
    return return_code_desc;

}
