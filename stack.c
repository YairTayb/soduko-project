#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "mainaux.h"


void initialize(stack *stk) {
    stk->count = 0;
    stk->top = NULL;
}

void push(stack *stk, move data) {
    elem *p = NULL;

    p = malloc(sizeof(elem));
    if (p == NULL) {
        printf(FUNCTION_FAILED, "malloc");
        exit(EXIT_FAILURE);
    }

    p->data = data;
    p->next = stk->top;
    stk->top = p;
    stk->count++;
}


move pop(stack *stk){
    move data;
    elem *p = NULL;

    data = stk->top->data;
    p = stk->top;
    stk->top = stk->top->next;
    stk->count--;
    free(p);
    return data;
}

move top(stack *stk){
    return stk->top->data;
}

int empty(stack *stk){
    if (stk->count == EMPTY)
        return TRUE;
    return FALSE;
}
