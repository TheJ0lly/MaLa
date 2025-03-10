#include <stdio.h>
#include <vm.h>

int main() {
    uint8_t progam[] = {
        OP_LOAD, 25, 0,
        OP_LOAD, 2, 1,
        OP_ADD, 0, 1,
        OP_PRINT, 0,
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