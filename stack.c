#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "mainaux.h"


/**
 * Initialize a stack
 * @param stk {stack*} The pointer to the stack to initialize
 */
void initialize(stack *stk) {
    stk->count = 0;
    stk->top = NULL;
}

/**
 * Push a move to the stack
 * @param stk {stack*} The pointer to the stack to push the move to
 * @param data {move} The move to push
 */
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

/**
 * Pop a move from the stack
 * @param stk {stack*} The pointer to the stack to pop from
 * @return {move} The popped move
 */
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

/**
 * Check if the stack is empty
 * @param stk {stack*} The pointer to the stack
 * @return {int} TRUE if empty, FALSE otherwise.
 */
int empty(stack *stk){
    if (stk->count == EMPTY)
        return TRUE;
    return FALSE;
}
