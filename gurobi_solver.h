#ifndef NIM_GUROBI_SOLVER_H
#define NIM_GUROBI_SOLVER_H

#include "data_models.h"

#define INDICES_ARR_LOC(i, j, k, dim1, dim2, dim3) dim2*dim3*i + dim3*j + k

returnCodeDesc solve_ILP(board game_board, int grid_height, int grid_width, int box_height, int box_width);

#endif
