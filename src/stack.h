#ifndef STACK_H
#define STACK_H

#define STACK_SIZE 1000

typedef struct LoopStack {
    int positions[STACK_SIZE];
    int top;
} LoopStack;

LoopStack *stack_init();
void stack_free(LoopStack *stack);

void stack_push(LoopStack *stack, int pos);
int stack_pop(LoopStack *stack);
#endif
