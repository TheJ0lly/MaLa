#include <stdio.h>
#include <vm.h>

int main() {
    uint8_t progam[] = {
        OP_LOAD, 25, 0,
        OP_LOAD, 2, 1,
        OP_ADD, 0, 1,
        OP_LOAD, 3, 2,
        OP_DIV, 0, 2,
        OP_LOAD, 2, 3,
        OP_MULT, 3, 0,
        OP_LOAD, 7, 4,
        OP_MOD, 3, 4, 5,
        OP_PRINT, 5,
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