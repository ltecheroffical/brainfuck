# Brainfuck Interpeter
This is an interpeter for the Brainfuck esolang.

# About Brainfuck
Brainfuck is a languauge made to be the smallest.

Commands:

- `>` - Move one position to the right
- `<` - Move one position to the left
- `+` - Increment the value at the current position
- `-` - Decrement the value at the current position
- `.` - Print the value at the current position
- `,` - Read a character and store it in the current position
- `[` - If the value at the current position is zero, jump to the matching `]`
- `]` - If the value at the current position is not zero, jump back to the matching `[`

# Usage

To run the interpreter, just run `brainfuck` in the terminal followed by the file you want to run.
This interpeter also supports debugging with `brainfuck <file> -d`

Debugging has a few commands:

- `h` - Print the help menu
- `q` - Quit the interpreter
- `s` - Print the state of the interpreter
- `d` - Detach the debugger
- `c` - Step to the next instruction
- `l` - Print the instruction stream

This interpreter also has some safety for out of bounds accesses.
Security can be disabled with `--no-index-protection` (may cause undefined behavior)
