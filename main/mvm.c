#include <stdio.h>
#include <vm.h>

uint8_t fib[] = {
    // Fibonacci starting numbers
    OP_LD1, 0, 0,
    OP_LD1, 1, 1,

    // Counters
    OP_LD1, 2, 1,
    OP_LD1, 3, 7,
    OP_LD1, 4, 1,

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
int fib_size = sizeof(fib);



uint8_t test[] = {
    OP_LD1, 0, 0x03,
    OP_LD1, 1, 0x0A,
    OP_NEG, 1,
    OP_MOD, 1, 0, 2,
    OP_PRINT, 2,
    OP_HALT,
};

int test_size = sizeof(test);


int main() {
    VM vm = {.ip = 0, .running = true, .err = NONE};
    load_program(&vm, test, test_size);

    while (vm.running) {
        execute_next(&vm);

        if (vm.err != NONE) {
            printf("%s\n", err_str(&vm));
        }
    }
}