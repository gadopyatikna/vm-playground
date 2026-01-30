#include "vm-register.h"

uint8_t addition_multiplication[] = {
    OP_LOAD_CONST, 0, 2,   // r0 = 2
    OP_LOAD_CONST, 1, 3,   // r1 = 3
    OP_ADD,        2, 0, 1,// r2 = r0 + r1
    OP_LOAD_CONST, 3, 4,   // r3 = 4
    OP_MUL,        4, 2, 3,// r4 = r2 * r3
    OP_HALT
};

// 0:  LOAD_CONST r0, 2
// 3:  LOAD_CONST r1, 3
// 6:  ADD        r2, r0, r1
// 10: JMP_IF_FALSE r2, 18
// 13: LOAD_CONST r3, 10
// 16: JMP        21
// 18: LOAD_CONST r3, 20
// 21: HALT
uint8_t jump[] = {
    OP_LOAD_CONST, 0, 2,      // 0
    OP_LOAD_CONST, 1, 3,      // 3
    OP_ADD,        2, 0, 1,   // 6
    OP_JMP_IF_FALSE, 2, 18,   // 10
    OP_LOAD_CONST, 3, 10,     // 13
    OP_JMP,        21,        // 16
    OP_LOAD_CONST, 3, 20,     // 18
    OP_HALT                   // 21
};

/*
main() {
    x = 1
    foo()
    z = x + 1
}

foo() {
    x = 2
    y = x + 1
}
*/
uint8_t program[] = {
    // --- main ---
    OP_LOAD_CONST,  0, 1,        // r0 = 1              //0
    OP_STORE_LOCAL, 0, 0,        // main.x = r0         //3

    OP_CALL,        22,          // call foo            //6

    OP_LOAD_LOCAL,  1, 0,        // r1 = main.x         //8
    OP_LOAD_CONST,  2, 1,                               //11
    OP_ADD,         3, 1, 2,    // r3 = r1 + r2         //14 
    OP_STORE_LOCAL, 1, 3,        // z = x + 1           //18

    OP_HALT,                                            //21

    // --- foo (address 22) ---
    OP_LOAD_CONST,  0, 2,        // r0 = 2              //22
    OP_STORE_LOCAL, 0, 0,        // foo.x = 2           //25

    OP_LOAD_LOCAL,  1, 0,                               //28
    OP_LOAD_CONST,  2, 1,                               //31   
    OP_ADD,         3, 1, 2,                            //34 
    OP_STORE_LOCAL, 1, 3,        // y = x + 1           //38

    OP_RET
};

