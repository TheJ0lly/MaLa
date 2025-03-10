#include <stdio.h>
#include <vm.h>

int main() {
    uint8_t progam[] = {
        OP_LOAD, 10, 0,
        OP_LOAD, 1, 1,
        OP_LOAD, 1, 2,
        OP_CMP, 1, 0,
        OP_JGR, 21, // change to HALT.
        OP_PRINT, 1,
        OP_ADD, 1, 2,
        OP_JMP, 9, // change to CMP.
        OP_HALT
    };

    VM vm = {.ip = 0, .running = true, .err = NONE};
    load_program(&vm, progam, sizeof(progam));

    while (vm.running) {
        execute_next(&vm);

        if (vm.err != NONE) {
            printf("%s\n", err_str(&vm));
        }
    }
}