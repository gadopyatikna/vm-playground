#include <stdio.h>
#include <stdint.h>

#define REG_COUNT 16
#define LOCAL_COUNT  16

typedef enum {
    OP_LOAD_CONST,
    OP_LOAD_LOCAL,
    OP_STORE_LOCAL,
    OP_ADD,
    OP_MUL,
    OP_JMP,
    OP_JMP_IF_FALSE,
    OP_HALT
} OpCode;

typedef struct {
    int locals[LOCAL_COUNT];
} Frame;

typedef struct {
    int regs[REG_COUNT];
    uint8_t* code;
    int ip;
    Frame frame;
} VM;

void vm_init(VM* vm, uint8_t* code) {
    for (int i = 0; i < REG_COUNT; i++) {
        vm->regs[i] = 0;
    }
    
    for (int i = 0; i < LOCAL_COUNT; i++)
        vm->frame.locals[i] = 0;

    vm->code = code;
    vm->ip = 0;
}

void vm_run(VM* vm) {
    for (;;) {
        OpCode op = vm->code[vm->ip++];

        switch (op) {
            case OP_LOAD_CONST: {
                uint8_t reg = vm->code[vm->ip++];
                int value   = vm->code[vm->ip++];
                vm->regs[reg] = value;
                break;
            }
            case OP_LOAD_LOCAL: {
                uint8_t dst   = vm->code[vm->ip++];
                uint8_t local = vm->code[vm->ip++];
                vm->regs[dst] = vm->frame.locals[local];
                break;
            }

            case OP_STORE_LOCAL: {
                uint8_t local = vm->code[vm->ip++];
                uint8_t src   = vm->code[vm->ip++];
                vm->frame.locals[local] = vm->regs[src];
                break;
            }

            case OP_ADD: {
                uint8_t dst = vm->code[vm->ip++];
                uint8_t a   = vm->code[vm->ip++];
                uint8_t b   = vm->code[vm->ip++];
                vm->regs[dst] = vm->regs[a] + vm->regs[b];
                break;
            }

            case OP_MUL: {
                uint8_t dst = vm->code[vm->ip++];
                uint8_t a   = vm->code[vm->ip++];
                uint8_t b   = vm->code[vm->ip++];
                vm->regs[dst] = vm->regs[a] * vm->regs[b];
                break;
            }
            
            case OP_JMP: {
                uint8_t target = vm->code[vm->ip++];
                vm->ip = target;
                break;
            }

            case OP_JMP_IF_FALSE: {
                uint8_t reg    = vm->code[vm->ip++];
                uint8_t target = vm->code[vm->ip++];

                if (vm->regs[reg] == 0) {
                    vm->ip = target;
                }
                break;
            }

            case OP_HALT:
                return;
        }
    }
}

int main() {
    // uint8_t program[] = {
    //     OP_LOAD_CONST, 0, 2,   // r0 = 2
    //     OP_LOAD_CONST, 1, 3,   // r1 = 3
    //     OP_ADD,        2, 0, 1,// r2 = r0 + r1
    //     OP_LOAD_CONST, 3, 4,   // r3 = 4
    //     OP_MUL,        4, 2, 3,// r4 = r2 * r3
    //     OP_HALT
    // };

    // 0:  LOAD_CONST r0, 2
    // 3:  LOAD_CONST r1, 3
    // 6:  ADD        r2, r0, r1
    // 10: JMP_IF_FALSE r2, 18
    // 13: LOAD_CONST r3, 10
    // 16: JMP        21
    // 18: LOAD_CONST r3, 20
    // 21: HALT

    uint8_t program[] = {
        OP_LOAD_CONST, 0, 2,      // 0
        OP_LOAD_CONST, 1, 3,      // 3
        OP_ADD,        2, 0, 1,   // 6
        OP_JMP_IF_FALSE, 2, 18,   // 10
        OP_LOAD_CONST, 3, 10,     // 13
        OP_JMP,        21,        // 16
        OP_LOAD_CONST, 3, 20,     // 18
        OP_HALT                // 21
    };

    VM vm;
    vm_init(&vm, program);
    vm_run(&vm);

    printf("result = %d\n", vm.regs[4]);
    return 0;
}
