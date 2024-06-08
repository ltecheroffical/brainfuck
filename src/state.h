#include <stdbool.h>

#ifndef STATE_H
#define STATE_H

#define MEMORY_SIZE 30000

typedef struct BrainfuckState {
    unsigned char memory[MEMORY_SIZE];
    unsigned char *ptr;
    bool memory_protection_enabled;
    bool debugger_enabled;
} BrainfuckState;

BrainfuckState *state_init();
void state_free(BrainfuckState *state);

void state_print(const BrainfuckState *state);
#endif
