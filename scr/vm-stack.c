#include <stdio.h>
#include <stdint.h>

typedef enum {
    OP_PUSH,
    OP_ADD,
    OP_MUL,
    OP_HALT
} OpCode;

typedef struct {
    int stack[256];
    int sp;
    uint8_t* code;
    int ip;
} VM;

void vm_init(VM* vm, uint8_t* code) {
    vm->sp = 0;
    vm->code = code;
    vm->ip = 0;
}

void push(VM* vm, int value) {
    vm->stack[vm->sp++] = value;
}

int pop(VM* vm) {
    return vm->stack[--vm->sp];
}

void vm_run(VM* vm) {
    for (;;) {
        OpCode op = vm->code[vm->ip++];

        switch (op) {
            case OP_PUSH: {
                int value = vm->code[vm->ip++];
                push(vm, value);
                break;
            }

            case OP_ADD: {
                int b = pop(vm);
                int a = pop(vm);
                push(vm, a + b);
                break;
            }

            case OP_MUL: {
                int b = pop(vm);
                int a = pop(vm);
                push(vm, a * b);
                break;
            }

            case OP_HALT:
                return;
        }
    }
}

int main() {
    uint8_t program[] = {
        OP_PUSH, 2,
        OP_PUSH, 3,
        OP_ADD,
        OP_PUSH, 4,
        OP_MUL,
        OP_HALT
    };

    VM vm;
    vm_init(&vm, program);
    vm_run(&vm);

    printf("result = %d\n", pop(&vm));
    return 0;
}
