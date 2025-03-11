#include <vm.h>

void load_program(VM *vm, uint8_t *program, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) {
        vm->memory[i] = program[i];
    }
}

void print_vm_state(VM *vm) {
    printf("== REGISTERS STATE ==\n");
    for (int i = 0; i < MAX_REGISTER; i++) {
        printf("  r%d:       %ld\n", i, vm->regs[i]);
    }

    printf("\n== MEMORY STATE ==\n");
        printf("  ip:        %d\n", vm->ip);
        printf("  last byte: %d\n", vm->memory[vm->ip]);
        printf("  error:     %s\n", err_str(vm));
        printf("  running:   %s\n", vm->running == true ? "true" : "false");
        printf("  compare:   %s\n", cmp_str(vm->cmp));
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

char *cmp_str(COMPARISON cmp){
    switch (cmp) {
    case LESS:
        return "LESS";
    case GREATER:
        return "GREATER";
    case EQUAL:
        return "EQUAL";
    default: // Should be unreachable
        return "UNREACHABLE";
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
    uint64_t address;


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

    case OP_NEG:
        reg1 = vm->memory[vm->ip++];
        vm->regs[reg1] = (~vm->regs[reg1]) + 1;
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

    case OP_MOA:
        reg1 = vm->memory[vm->ip++];
        reg2 = vm->memory[vm->ip++];

        address = vm->regs[reg2];
        vm->memory[address] = vm->regs[reg1];

        break;

    case OP_LFA:
        reg1 = vm->memory[vm->ip++];
        reg2 = vm->memory[vm->ip++];

        address = vm->regs[reg2];
        vm->regs[reg1] = vm->memory[address];
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
        address = vm->memory[vm->ip++];
        vm->ip = address;
        break;

    case OP_JEQ:
        address = vm->memory[vm->ip++];

        if (vm->cmp == EQUAL)
            vm->ip = address;

        break;
    
    case OP_JNE:
        address = vm->memory[vm->ip++];

        if (vm->cmp != EQUAL)
            vm->ip = address;

        break;

    case OP_JGR:
        address = vm->memory[vm->ip++];
        
        if (vm->cmp == GREATER)
            vm->ip = address;

        break;
    
    case OP_JLE:
        address = vm->memory[vm->ip++];

        if (vm->cmp == LESS)
            vm->ip = address;

        break;


    default:
        vm->err = UNKNOWN;
        vm->running = false;
        break;
    }
}