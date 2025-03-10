#include <vm.h>

void load_program(VM *vm, uint8_t *program, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) {
        vm->memory[i] = program[i];
    }
}

char *err_str(VM *vm) {
    switch (vm->err) {
    case NONE:
        return "None";

    case UNKNOWN:
        return "Unknown Operation";
    
    case OVERFLOW:
        return "Memory Overflow";
    
    case UNDERFLOW:
        return "Memory Underflow";

    default:
        return "Unknown Error";
    }
}

void execute_next(VM *vm) {
    if (vm->ip + 1 >= MAX_MEMORY) {
        vm->err = OVERFLOW;
        vm->running = false;
        return;
    }

    uint8_t op = vm->memory[vm->ip++];
    uint8_t value, reg;

    switch (op) {
    case OP_HALT:
        vm->running = false;
        break;
    
    case OP_PRINT:
        reg = vm->memory[vm->ip++];
        printf("%ld\n", vm->regs[reg]);
        break;

    case OP_LOAD:
        value = vm->memory[vm->ip++];
        reg = vm->memory[vm->ip++];
        vm->regs[reg] = value;
        break;

    case OP_ADD:
        reg = vm->memory[vm->ip++];
        value = vm->memory[vm->ip++];
        vm->regs[reg] += vm->regs[value];
        break;

    case OP_SUB:
        reg = vm->memory[vm->ip++];
        value = vm->memory[vm->ip++];
        vm->regs[reg] -= vm->regs[value];
        break;
    
    default:
        vm->err = UNKNOWN;
        vm->running = false;
        break;
    }
}