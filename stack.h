#ifndef SODUKO_PROJECT_STACK_H
#define SODUKO_PROJECT_STACK_H

#include "data_models.h"
#include "consts.h"

void initialize(stack *stk);

void push(stack *stk, move data);

move pop(stack *stk);

move top(stack *stk);

int empty(stack *stk);

#endif
