#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "state.h"

BrainfuckState *state_init() {
    BrainfuckState *state = malloc(sizeof(BrainfuckState));
    memset(state->memory, 0, sizeof(state->memory));
    state->ptr = state->memory;
    state->memory_protection_enabled = true;
    state->debugger_enabled = false;
    return state;
}

void state_free(BrainfuckState *state) {
    free(state);
}

void state_print(const BrainfuckState *state) {
    fprintf(stderr, "** DEBUG PRINT **\n");

    int pointer_index = state->ptr - state->memory;
    fprintf(stderr, "Current index: \t\t%d\n", pointer_index);
    
    if (state->ptr < state->memory || state->ptr > state->memory + MEMORY_SIZE - 1) {
        fprintf(stderr, "(Current index out of bounds)\n");
    }

    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (i == pointer_index) {
            fprintf(stderr, "> Index %d:\t\t%d\n", i, state->memory[i]);
        } else if (state->memory[i] != 0) {
            fprintf(stderr, "  Index %d:\t\t%d\n", i, state->memory[i]);
        }
    }
    fprintf(stderr, "** END DEBUG PRINT **\n");
}
