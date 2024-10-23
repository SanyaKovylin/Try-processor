#ifndef CMD_H_INCLUDED
#define CMD_H_INCLUDED

#include <stdio.h>
#include "..\HellStack/Include/stack.h"

const int vsize   = 8;
typedef double vtype;
const int cmdsize = 1;
const int scale = 2;


typedef enum Commands {
    NON_CMD  =  0,
    CMD_PUSH =  1,
    CMD_OUT  =  2,
    CMD_IN   =  3,
    CMD_DUMP =  4,
    CMD_HLT  =  5,
    CMD_ADD  =  6,
    CMD_SUB  =  7,
    CMD_MUL  =  8,
    CMD_DIV  =  9,
    CMD_SQRT = 10,
    CMD_SIN  = 11,
    CMD_COS  = 12,
    CMD_JMP  = 13,
    CMD_JA   = 14,
    CMD_JAE  = 15,
    CMD_JB   = 16,
    CMD_JBE  = 17,
    CMD_JE   = 18,
    CMD_JNE  = 19,
    CMD_CALL = 20,
    CMD_RET  = 21,
    CMD_POP  = 22,
    CMD_DRAW = 23,
    CMD_REDP = 24,
    CMD_COUNT,
} cmd_t;

const int ncmds = CMD_COUNT-1;

typedef enum CompilerErrors {
    CAT_CONDITION         = 0, //MEOW !!! MURRRRRRRRR !!!
    IN_YOUR_FUCKING_STACK = 1, //Debil pisal etot stack
    I_CHTO_MNE_S_THIS_DO  = 2, //check function arguments !!!
    YOUR_READER_ERROR     = 3,
    NE_PODELISH_NA_NICHTO = 4,
    COMANDA_KRINGE        = 5,
    SAM_PRIDUMAL_REG_TAKOY= 6,
    THIS_PAR_IS_INVALID   = 7,
} err_t;

const int MaxCmdSize     =  8;
const int LenOfCmdPhrase =  4;
const int MaxCommands    = 30;

typedef struct Command {
    cmd_t cmd        : 5;
    unsigned int mem : 1;
    unsigned int reg : 1;
    unsigned int ico : 1;
} com_t;

#endif //CMD_H_INCLUDED
