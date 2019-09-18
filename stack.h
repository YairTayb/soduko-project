/*Stack module.
This module consists of the methods used to manage the stack,
in order to implement non recursive board solving methods.*/
#ifndef SODUKO_PROJECT_STACK_H
#define SODUKO_PROJECT_STACK_H

#include "data_models.h"
#include "consts.h"

void initialize(stack *stk);

void push(stack *stk, move data);

move pop(stack *stk);

int empty(stack *stk);

#endif
