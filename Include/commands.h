#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

#include "cmd.h"

vtype DefaultTypeValue = 0;

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

struct CmdNum GetFunc[] {
    CMD_PUSH, func_push,
    CMD_OUT , func_out ,
    CMD_IN  , func_in  ,
    CMD_DUMP, func_dump,
    CMD_HLT , func_halt,
    CMD_ADD , func_add ,
    CMD_SUB , func_sub ,
    CMD_MUL , func_mul ,
    CMD_DIV , func_div ,
    CMD_SQRT, func_sqrt,
    // CMD_SIN ,
    // CMD_COS ,
};

void print_int(FILE *fw, void* elem);

int (*castfromstr) (char *str, vtype* val) = NULL;

#ifndef NPRINT
    #define FUNC_PRINT(value) printf("%s value = %g\n", __func__, value);
#else
    #define FUNC_PRINT(value) ;
#endif
#endif //COMMANDS_H_INCLUDED
