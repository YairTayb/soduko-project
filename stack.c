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

/*
int main(){
    stack s;
    struct move m1;
    struct move m2;
    struct move m3;
    struct move m4;
    m1.row = 0;
    m1.col = 0;
    m1.old_val = 0;
    m1.new_val = 1;

    m2.row = 0;
    m2.col = 0;
    m2.old_val = 1;
    m2.new_val = 2;

    m3.row = 0;
    m3.col = 0;
    m3.old_val = 1;
    m3.new_val = 4;

    initialize(&s);
    push(&s, m1);
    push(&s, m2);
    push(&s, m3);

    while (!empty(&s)){
        m4 = pop(&s);
        printf("%d %d %d %d\n", m4.row, m4.col, m4.old_val, m4.new_val);
    }
    return 1;

}
*/
