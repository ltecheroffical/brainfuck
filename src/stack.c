#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

LoopStack *stack_init() {
    LoopStack *stack = malloc(sizeof(LoopStack));
    stack->top = -1;
    return stack;
}

void stack_free(LoopStack *stack) {
    free(stack);
}

void stack_push(LoopStack *stack, int position) {
    if (stack->top < STACK_SIZE - 1) {
        stack->positions[++stack->top] = position;
    } else {
        fprintf(stderr, "Stack overflow\n");
    }
}

int stack_pop(LoopStack *stack) {
    if (stack->top >= 0) {
        return stack->positions[stack->top--];
    } else {
        fprintf(stderr, "Stack underflow\n");
        return stack->positions[0];
    }
}
