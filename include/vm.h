#ifndef __VM_H__
#define __VM_H__

#define MAX_MEMORY 4096
#define MAX_REGISTER 10

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef enum {
    // 0 variables
    OP_HALT = 1,

    // 1 variable
    OP_PRINT,

    // 2 variables
    OP_LOAD,

    // 2 variables
    OP_MOV,

    // Maths
    // 2 variables
    OP_ADD,
    // 2 variables
    OP_SUB,
    // 2 variables
    OP_MULT,
    // 2 variables
    OP_DIV,
    // 3 variables
    OP_MOD,

    // Compare
    // 2 variables
    OP_CMP,

    // Jumps
    // 1 variable
    OP_JMP,
    // 1 variables
    OP_JEQ,
    // 1 variables
    OP_JNE,
    // 1 variables
    OP_JGR,
    // 1 variables
    OP_JLE,
} OP;

typedef enum {
    NONE,
    UNKNOWN,
    OVERFLOW,
    UNDERFLOW,
} ERROR;

typedef enum {
    LESS,
    EQUAL,
    GREATER,
} COMPARISON;

typedef struct {
    uint8_t memory[MAX_MEMORY];
    uint32_t ip;
    int64_t regs[MAX_REGISTER];
    ERROR err;
    bool running;
    COMPARISON cmp;
} VM;

void   load_program(VM *vm, uint8_t *program, uint32_t size);
char*  err_str(VM *vm);
void   execute_next(VM *vm);


#endif //__VM_H__