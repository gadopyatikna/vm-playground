
#ifndef VM_H
#define VM_H

#include <stdint.h>
#define REG_COUNT 16
#define LOCAL_COUNT  16
#define MAX_FRAMES  8

typedef enum {
    OP_LOAD_CONST,
    OP_LOAD_LOCAL,
    OP_STORE_LOCAL,
    OP_ADD,
    OP_MUL,
    OP_JMP,
    OP_JMP_IF_FALSE,
    OP_CALL,
    OP_RET,
    OP_HALT
} OpCode;

typedef struct {
    int locals[LOCAL_COUNT];
    int return_ip;
} Frame;

typedef struct {
    int regs[REG_COUNT];
    uint8_t* code;
    int ip;
    int frame_top;
    Frame frames[MAX_FRAMES];
} VM;

#endif // VM_H