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

    // 1 variable: register
    OP_PRINT,

    // 2 variables: register | value
    OP_LD1,

    // 3 variables: register | byte | byte 
    OP_LD2,

    // 5 variables: register | byte | byte | byte | byte 
    OP_LD4,

    // 9 variables: register | byte | byte | byte | byte | byte | byte | byte | byte 
    OP_LD8,

    // 1 variable: register to negate
    OP_NEG,

    // 2 variables: register to OR into -> register value
    OP_OR,

    // 2 variables: register to AND into -> register value
    OP_AND,

    // 2 variables: register to XOR into -> register value
    OP_XOR,

    // 2 variables: bits | register
    OP_SHL,

    // 2 variables: bits | register
    OP_SHR,

    // 2 variables: register value -> register
    OP_MOV,

    // 2 variables: register value -> register that holds location.
    OP_MOA,

    // 2 variables: register to store | register that holds the location
    OP_LFA,

    // Maths
    // 2 variables: register to add into -> register value
    OP_ADD,
    // 2 variables: register to sub into -> register value
    OP_SUB,
    // 2 variables: register to mult into -> register value
    OP_MLT,
    // 2 variables: register to div into -> register value
    OP_DIV,
    // 3 variables: arg1 % arg2 -> arg3
    OP_MOD,

    // Compare
    // 2 variables: primary registers | second register
    OP_CMP,

    // Jumps
    // 1 variable: location
    OP_JMP,
    // 1 variable: location
    OP_JEQ,
    // 1 variable: location
    OP_JNE,
    // 1 variable: location
    OP_JGR,
    // 1 variable: location
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
void   print_vm_state(VM *vm);
char*  err_str(VM *vm);
char*  cmp_str(COMPARISON cmp);
void   execute_next(VM *vm);


#endif //__VM_H__