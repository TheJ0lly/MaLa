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
    // OP_JMP, 3,
    OP_HALT,
};
int test_size = sizeof(test);

uint8_t arr_test[] = {
    // Counter starting at 1
    OP_LD1, 1, 1,
    // Check value at 10
    OP_LD1, 2, 10,

    // Increment value
    OP_LD1, 3, 1,

    // Pointer value at 50
    OP_LD1, 4, 0x32,

    // R5 is the moving pointer.
    OP_MOV, 4, 5,

    // Compare R1 with R2
    OP_CMP, 1, 2,
    OP_JGR, 36, // To halt.

    // We put the R1 value at memory location.
    OP_MOA, 1, 5,

    // We load into R9 the value at location R5.
    OP_LFA, 9, 5,
    OP_PRINT, 9,

    // We increment R5 with R3 value.
    OP_ADD, 5, 3,

    // We increment the counter.
    OP_ADD, 1, 3,

    // We jump to the comparison
    OP_JMP, 15,

    // We stop
    OP_HALT,
};

int arr_test_size = 37;



int main() {
    VM vm = {.ip = 0, .running = true, .err = NONE};
    load_program(&vm, arr_test, arr_test_size);

    while (vm.running) {
        execute_next(&vm);

        if (vm.err != NONE) {
            print_vm_state(&vm);
        }
    }
}