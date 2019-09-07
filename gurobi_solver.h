#ifndef NIM_GUROBI_SOLVER_H
#define NIM_GUROBI_SOLVER_H

#include "data_models.h"

returnCodeDesc solve_gurobi(board game_board, int grid_height, int grid_width, int box_height, int box_width,
                            linear_solving_mode solving_mode, double *guess_scores);

#endif
