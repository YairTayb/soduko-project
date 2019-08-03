#ifndef SODUKO_PROJECT_STACK_H
#define SODUKO_PROJECT_STACK_H

#define EMPTY 0

typedef struct move {
    int value;
    int row;
    int col;
} move;

typedef struct elem {
    move data;
    struct elem *next;
} elem;

typedef struct stack {
    int count;
    elem *top;
} stack;

void initialize(stack *stk);
void push(stack *stk, move data);
struct move pop(stack *stk);
struct move top(stack *stk);
int empty(stack *stk);

#endif
