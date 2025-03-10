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
    uint8_t reg1, reg2;
    uint8_t value;


    switch (op) {
    case OP_HALT:
        vm->running = false;
        break;
    
    case OP_PRINT:
        reg1 = vm->memory[vm->ip++];
        printf("%ld\n", vm->regs[reg1]);
        break;

    case OP_LD1:
        reg1 = vm->memory[vm->ip++];
        value = vm->memory[vm->ip++];
        vm->regs[reg1] = value;
        break;
    
    case OP_LD2:
        reg1 = vm->memory[vm->ip++];
        for (int i = 0; i < 2; i++) {
            value = vm->memory[vm->ip++];
            vm->regs[reg1] <<= 8;
            vm->regs[reg1] |= value;
        }
        break;

    case OP_LD4:
        reg1 = vm->memory[vm->ip++];
        for (int i = 0; i < 4; i++) {
            value = vm->memory[vm->ip++];
            vm->regs[reg1] <<= 8;
            vm->regs[reg1] |= value;
        }
        break;

    case OP_LD8:
        reg1 = vm->memory[vm->ip++];
        for (int i = 0; i < 8; i++) {
            value = vm->memory[vm->ip++];
            vm->regs[reg1] <<= 8;
            vm->regs[reg1] |= value;
        }
        break;
    
    case OP_OR:
        reg1 = vm->memory[vm->ip++];
        reg2 = vm->memory[vm->ip++];

        vm->regs[reg1] |= vm->regs[reg2];
        break;

    case OP_AND:
        reg1 = vm->memory[vm->ip++];
        reg2 = vm->memory[vm->ip++];

        vm->regs[reg1] &= vm->regs[reg2];
        break;
    case OP_XOR:
        reg1 = vm->memory[vm->ip++];
        reg2 = vm->memory[vm->ip++];

        vm->regs[reg1] ^= vm->regs[reg2];
        break;
    case OP_SHL:
        value = vm->memory[vm->ip++];
        reg1 = vm->memory[vm->ip++];

        vm->regs[reg1] <<= value;
        break;
    case OP_SHR:
        value = vm->memory[vm->ip++];
        reg1 = vm->memory[vm->ip++];

        vm->regs[reg1] >>= value;
        break;

    case OP_MOV:
        reg1 = vm->memory[vm->ip++];
        reg2 = vm->memory[vm->ip++];

        vm->regs[reg2] = vm->regs[reg1];
        break;

    case OP_ADD:
        reg1 = vm->memory[vm->ip++];
        reg2 = vm->memory[vm->ip++];
        vm->regs[reg1] += vm->regs[reg2];
        break;

    case OP_SUB:
        reg1 = vm->memory[vm->ip++];
        reg2 = vm->memory[vm->ip++];
        vm->regs[reg1] -= vm->regs[reg2];
        break;
    
    case OP_MLT:
        reg1 = vm->memory[vm->ip++];
        reg2 = vm->memory[vm->ip++];
        {
            // We first store the actual result of the multiplication
            // then we store it in the register so as to not lose bits.
            double rez = (double)vm->regs[reg1] * (double)vm->regs[reg2];
            vm->regs[reg1] = rez;
        }
        break;
    
    case OP_DIV:
        reg1 = vm->memory[vm->ip++];
        reg2 = vm->memory[vm->ip++];
        {
            // We first store the actual result of the division
            // then we store it in the register so as to not lose bits.
            double rez = (double)vm->regs[reg1] / (double)vm->regs[reg2];
            vm->regs[reg1] = rez;
        }
        break;
    
    case OP_MOD:
        reg1 = vm->memory[vm->ip++];
        reg2 = vm->memory[vm->ip++];
        // Here `value` represents the third register
        value = vm->memory[vm->ip++];
        
        vm->regs[value] = vm->regs[reg1] % vm->regs[reg2];
        break;

    case OP_CMP:
        // We compare 2 registers, where the first is the subject of 
        // comparison.
        reg1 = vm->memory[vm->ip++];
        reg2 = vm->memory[vm->ip++];

        if (vm->regs[reg1] > vm->regs[reg2])
            vm->cmp = GREATER;
        else if (vm->regs[reg1] < vm->regs[reg2])
            vm->cmp = LESS;
        else
            vm->cmp = EQUAL;
        break;

    case OP_JMP:
        // Here `value` represents the location to jump to.
        value = vm->memory[vm->ip++];
        vm->ip = value;
        break;

    case OP_JEQ:
        // Here `value` represents the location to jump to.
        value = vm->memory[vm->ip++];

        if (vm->cmp == EQUAL)
            vm->ip = value;

        break;
    
    case OP_JNE:
        // Here `value` represents the location to jump to.
        value = vm->memory[vm->ip++];

        if (vm->cmp != EQUAL)
            vm->ip = value;

        break;

    case OP_JGR:
        // Here `value` represents the location to jump to.
        value = vm->memory[vm->ip++];
        
        if (vm->cmp == GREATER)
            vm->ip = value;

        break;
    
    case OP_JLE:
        // Here `value` represents the location to jump to.
        value = vm->memory[vm->ip++];

        if (vm->cmp == LESS)
            vm->ip = value;

        break;


    default:
        vm->err = UNKNOWN;
        vm->running = false;
        break;
    }
}