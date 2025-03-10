#include <stdio.h>
#include <vm.h>

uint8_t fib[] = {
    // Fibonacci starting numbers
    OP_LOAD, 0, 0,
    OP_LOAD, 1, 1,

    // Counters
    OP_LOAD, 1, 2,
    OP_LOAD, 7, 3,
    OP_LOAD, 1, 4,

    OP_CMP, 2, 3,
    OP_JGR, 36, // To halt

    // Temp: 1 in 5
    OP_MOV, 1, 5,
    OP_ADD, 1, 0,
    OP_MOV, 5, 0,

    OP_PRINT, 1,

    // Increase the counter
    OP_ADD, 2, 4,
    OP_JMP, 15,

    OP_HALT,
};
int fib_size = 37;

int main() {


    VM vm = {.ip = 0, .running = true, .err = NONE};
    load_program(&vm, fib, fib_size);

    while (vm.running) {
        execute_next(&vm);

        if (vm.err != NONE) {
            printf("%s\n", err_str(&vm));
        }
    }
}