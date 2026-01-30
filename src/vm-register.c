#include <stdio.h>
#include "vm-register.h"
#include "programs.h"

void vm_init(VM* vm, uint8_t* code) {
    for (int i = 0; i < REG_COUNT; i++) {
        vm->regs[i] = 0;
    }
    
    for (int f = 0; f < MAX_FRAMES; f++) {
        for (int i = 0; i < LOCAL_COUNT; i++) {
            vm->frames[f].locals[i] = 0;
        }
    }

    vm->code = code;
    vm->ip = 0;
    vm->frame_top = 0;
}

void vm_run(VM* vm) {
    for (;;) {
        OpCode op = vm->code[vm->ip++];
        switch (op) {
            case OP_LOAD_CONST: {
                uint8_t reg = vm->code[vm->ip++];
                int value   = vm->code[vm->ip++];
                vm->regs[reg] = value;

                printf(" op: OP_LOAD_CONST reg=%d value=%d\n", reg, value);
                break;
            }
            case OP_LOAD_LOCAL: {
                uint8_t dst   = vm->code[vm->ip++];
                uint8_t local = vm->code[vm->ip++];
                vm->regs[dst] = vm->frames[vm->frame_top].locals[local];
                printf(" op: OP_LOAD_LOCAL dst=%d local=%d value=%d\n", dst, local, vm->frames[vm->frame_top].locals[local]);
                break;
            }

            case OP_STORE_LOCAL: {
                uint8_t local = vm->code[vm->ip++];
                uint8_t src   = vm->code[vm->ip++];
                vm->frames[vm->frame_top].locals[local] = vm->regs[src];
                printf(" op: OP_STORE_LOCAL local=%d src=%d value=%d\n", local, src,  vm->regs[src]);
                break;
            }

            case OP_ADD: {
                uint8_t dst = vm->code[vm->ip++];
                uint8_t a   = vm->code[vm->ip++];
                uint8_t b   = vm->code[vm->ip++];
                vm->regs[dst] = vm->regs[a] + vm->regs[b];
                printf(" op: OP_ADD dst=%d a=%d b=%d value=%d\n", dst, a, b, vm->regs[dst]);
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

            case OP_CALL: {
                uint8_t target = vm->code[vm->ip++];

                vm->frames[vm->frame_top].return_ip = vm->ip;
                vm->frame_top++;

                // очистка locals нового frame
                for (int i = 0; i < LOCAL_COUNT; i++)
                    vm->frames[vm->frame_top].locals[i] = 0;

                vm->ip = target;
                break;
            }

            case OP_RET: {
                vm->frame_top--;
                vm->ip = vm->frames[vm->frame_top].return_ip;
                break;
            }

            case OP_HALT:
                return;
        }
    }
}



int main() {



    VM vm;
    vm_init(&vm, program);
    vm_run(&vm);

    printf("result = %d\n", vm.regs[0]);
    return 0;
}
