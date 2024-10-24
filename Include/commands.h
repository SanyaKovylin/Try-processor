#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

#include "cmd.h"

vtype DefaultTypeValue = 0;

const size_t RegLen = sizeof("ax");
const int nreg = 4;

const int withcolor = 0;
const int LenRam = 72*160*(withcolor + 1);
// const int LenRam = 200*(withcolor + 1);
const int RamRow = 160*(withcolor + 1);
// const int RamRow = 20*(withcolor + 1);

typedef struct Conditions {
    Stack *Buffer;
    Stack *Calls;
    vtype *RAM;
    vtype *Regs;
    char *Ips;
    size_t ip;
    int (*decodefunc) (char *LineWithValue, vtype *elem);
    bool tocontinue;
    com_t cmd;
    size_t ipm;
} cond_t;

struct CmdNum {
    cmd_t command;
    err_t (*cmdfunc)(cond_t *MainConditions);
};

static err_t func_push(cond_t *maincons);
static err_t func_out (cond_t *maincons);
static err_t func_in  (cond_t *maincons);
static err_t func_dump(cond_t *maincons);
static err_t func_halt(cond_t *maincons);
static err_t func_add (cond_t *maincons);
static err_t func_sub (cond_t *maincons);
static err_t func_mul (cond_t *maincons);
static err_t func_div (cond_t *maincons);
static err_t func_sqrt(cond_t *maincons);
static err_t func_sin (cond_t *maincons);
static err_t func_cos (cond_t *maincons);
static err_t func_jmp (cond_t *maincons);
static err_t func_ja  (cond_t *maincons);
static err_t func_jae (cond_t *maincons);
static err_t func_jb  (cond_t *maincons);
static err_t func_jbe (cond_t *maincons);
static err_t func_je  (cond_t *maincons);
static err_t func_jne (cond_t *maincons);
static err_t func_call(cond_t *maincons);
static err_t func_ret (cond_t *maincons);
static err_t func_pop (cond_t *maincons);
static err_t func_draw(cond_t *maincons);
static err_t func_redp(cond_t *maincons);

struct CmdNum GetFunc[] { // awaitsReg // RequiuresReg
    CMD_PUSH , func_push,
    CMD_OUT  , func_out ,
    CMD_IN   , func_in  ,
    CMD_DUMP , func_dump,
    CMD_HLT  , func_halt,
    CMD_ADD  , func_add ,
    CMD_SUB  , func_sub ,
    CMD_MUL  , func_mul ,
    CMD_DIV  , func_div ,
    CMD_SQRT , func_sqrt,
    CMD_SIN  , func_sin,
    CMD_COS  , func_cos,
    CMD_JMP  , func_jmp,
    CMD_JA   , func_ja,
    CMD_JAE  , func_jae,
    CMD_JB   , func_jb,
    CMD_JBE  , func_jbe,
    CMD_JE   , func_je,
    CMD_JNE  , func_jne,
    CMD_CALL , func_call,
    CMD_RET  , func_ret,
    CMD_POP  , func_pop,
    CMD_DRAW , func_draw,
    CMD_REDP , func_redp,
};

void print_int(FILE *fw, void* elem);

int (*castfromstr) (char *str, vtype* val) = NULL;

#ifndef NPRINT
    #define FUNC_PRINT(value) printf("%s value = %d\n", __func__, value);
#else
    #define FUNC_PRINT(value) ;
#endif
#endif //COMMANDS_H_INCLUDED
