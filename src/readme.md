vm-stack

AST отсутствует полностью
ip — явный
stack — явный
байткод — данные, не структура
switch(opcode) — классический dispatch loop

vm-register

LOAD_CONST r0, 2
LOAD_CONST r1, 3
ADD        r2, r0, r1
LOAD_CONST r3, 4
MUL        r4, r2, r3
HALT

Несколько тонких, но ключевых моментов:
    Нет push/pop → меньше инструкций → меньше dispatch
    Значения имеют адреса → можно анализировать lifetimes
    Инструкция ≈ выражение → проще оптимизации
    VM ближе к CPU → JIT почти напрямую маппится
    Если ты мысленно представишь SSA, ты увидишь, что r0…r4 — уже почти она.

Сравнение на уровне байткода (очень показательно)
Стековая
PUSH 2
PUSH 3
ADD
PUSH 4
MUL

Регистровая
r2 = r0 + r1
r4 = r2 * r3

Куда логично двигаться дальше
Следующие шаги уже почти неизбежны:
    JMP, JMP_IF_FALSE
    frame + locals
    CALL, RET
    SSA-представление
    dead code elimination
    threaded / direct dispatch
    tiny JIT (хотя бы в asm)

Исходный пункт:
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

нет CALL / RET
нет стека фреймов
нет механизма создания / уничтожения frame
нет даже концепции «текущий frame меняется»

gcc -Wall -Wextra -g \
    vm-register.c \
    -o vm