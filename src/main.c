#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "state.h"
#include "stack.h"

const char HELP_MESSAGE[] = "Usage: %s <filename>\n"
                            "\n"
                            "--no-index-protection\tDisable index protection\n"
                            "--debug, -d\tDebug the brainfuck code\n";

const char DEBUG_HELP_MESSAGE[] = "DEBUGGER HELP\n"
                                  "\n"
                                  "h\tPrint this message\n"
                                  "q\tQuit the debugger\n"
                                  "s\tPrint the state of the memory\n"
                                  "d\tDisable the debugger\n"
                                  "c\tStep to the next instruction\n"
                                  "i\tPrint the instruction stream\n";


void process_instruction(BrainfuckState *state, char c, LoopStack *loop_stack, FILE *fp) {
    switch (c) {
        case '>':
            if (state->memory_protection_enabled && state->ptr == state->memory + MEMORY_SIZE - 1) {
                fprintf(stderr, "Attempted to access memory outside of bounds!\n");
                state_print(state);
            } else {
                state->ptr++;
            }
            break;
        case '<':
            if (state->memory_protection_enabled && state->ptr == state->memory) {
                fprintf(stderr, "Attempted to access memory outside of bounds!\n");
                state_print(state);
            } else {
                state->ptr--;
            }
            break;
        case '+':
            (*state->ptr)++;
            break;
        case '-':
            (*state->ptr)--;
            break;
        case '.':
            putchar(*state->ptr);
            break;
        case ',':
            *state->ptr = getchar();
            break;
        case '!':
            state_print(state);
            break;
        case '[':
            if (*state->ptr == 0) {
                int loop_count = 1;
                while (loop_count > 0) {
                    c = fgetc(fp);
                    if (c == '[') loop_count++;
                    if (c == ']') loop_count--;
                }
            } else {
                stack_push(loop_stack, ftell(fp) - 1);
            }
            break;
        case ']':
            if (*state->ptr != 0) {
                fseek(fp, stack_pop(loop_stack), SEEK_SET);
            } else {
                stack_pop(loop_stack);
            }
            break;
    }
}

void print_instruction_stream(FILE *fp) {
    // Get where the file is
    int file_pos = ftell(fp);

    // Go to the beginning of the file
    fseek(fp, 0, SEEK_SET);

    // Read the file
    char c;
    while ((c = fgetc(fp)) != EOF) {
        // Is that our position?
        if (ftell(fp) == file_pos) {
            // Print it colored
            fprintf(stderr, "\x1b[32m%c\x1b[0m", c);
        } else {
            fprintf(stderr, "%c", c);
        }
    }

    // Revert to the original position
    fseek(fp, file_pos, SEEK_SET);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf(HELP_MESSAGE, argv[0]);
        return 1;
    }

    BrainfuckState *state = state_init();

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror(argv[1]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--no-index-protection") == 0) {
            fprintf(stderr, "Warning: Index protection has been disabled, this means that instructions won't be checked.\n");
            state->memory_protection_enabled = false;
        } else if (strcmp(argv[i], "--debug") == 0 || strcmp(argv[i], "-d") == 0) {
            state->debugger_enabled = true;
        }
    }

    LoopStack *loop_stack = stack_init();

    char c;
   
    int continue_count = 0;
    bool continue_count_in_progress = true;

    while ((c = fgetc(fp)) != EOF) {
        if (state->debugger_enabled) {
            while (continue_count <= 0 || continue_count_in_progress) {
                fprintf(stderr, "(BrainDbg) ");
                char input = getchar();

                switch (input) {
                    case 'h':
                        fprintf(stderr, DEBUG_HELP_MESSAGE);
                        continue_count_in_progress = false;
                        break;
                    case 'q':
                        fprintf(stderr, "[Terminated]\n");
                        fclose(fp);
                        continue_count_in_progress = false;
                        return 0;
                    case 's':
                        state_print(state);
                        continue_count_in_progress = false;
                        break;
                    case 'd':
                        state->debugger_enabled = false;
                        continue_count++;
                        continue_count_in_progress = false;
                        break;
                    case 'c':
                        continue_count_in_progress = true;
                        break;
                    case 'i':
                        fprintf(stderr, "PROGRAM INSTRUCTION STREAM\n");
                        print_instruction_stream(fp);
                        continue_count_in_progress = false;
                        break;
                    default:
                        if (continue_count_in_progress && input >= '0' && input <= '9') {
                            // add this digit to the continue count
                            continue_count = continue_count * 10 + (input - '0');
                        } else if (continue_count_in_progress) {
                            continue_count_in_progress = false;
                        }
                        break;
                }
                fprintf(stderr, "\n");
            }
            if (continue_count > 0) {
                continue_count--;
            }
        }
        process_instruction(state, c, loop_stack, fp);
    }

    stack_free(loop_stack);
    state_free(state);

    fclose(fp);
    fprintf(stderr, "[Terminated]\n");
    return 0;
}

