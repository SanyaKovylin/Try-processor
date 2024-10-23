#ifndef N_PARSER_H_INCLUDED
#define N_PARSER_H_INCLUDED

#include "cmd.h"

struct CmdName {
    cmd_t command;
    const char *cmdname;
};

const char *Skip4el(const char* CmdName);

#define COMD(name) {name, Skip4el(#name)}

const struct CmdName FindFunc[] {
    COMD(CMD_PUSH),
    COMD(CMD_OUT),
    COMD(CMD_IN),
    COMD(CMD_DUMP),
    COMD(CMD_HLT),
    COMD(CMD_ADD),
    COMD(CMD_SUB),
    COMD(CMD_MUL),
    COMD(CMD_DIV),
    COMD(CMD_SQRT),
    COMD(CMD_SIN),
    COMD(CMD_COS),
    COMD(CMD_JMP),
    COMD(CMD_JA),
    COMD(CMD_JAE),
    COMD(CMD_JB),
    COMD(CMD_JBE),
    COMD(CMD_JE),
    COMD(CMD_JNE),
    COMD(CMD_CALL),
    COMD(CMD_RET),
    COMD(CMD_POP),
    COMD(CMD_DRAW),
};



int (*castfromstr) (char *str, vtype* val) = NULL;

typedef struct Mark {
    char name[10];
    size_t ptr;
} mark_t;

typedef struct Mk {
    mark_t *marks;
    size_t size;
    size_t ptr;
} mk_t;

const int nmarks = 8;

// 000 - no args
// 001 - only vt
// 010 - only rg
// 011 - rg + vt
// 100 - rg + rg
// 101 - only ip
// 110 - ip + val
// 111 - rg or val

typedef struct BufferConditions {
    char *buffer;
    char *wbuf;
    size_t rdr;
    size_t wrt;
} buf_t;

enum regn {
    NON_REG= 0,
    REG_AX = 1,
    REG_BX = 2,
    REG_CX = 3,
    REG_DX = 4,
    COUNTREG,
};

const int nreg = COUNTREG - 1;
const int regsize = sizeof(char);

typedef struct Registr {
    regn value;
    const char *name;
} rgtr_t;

const rgtr_t RegArray[]  = {
    COMD(REG_AX),
    COMD(REG_BX),
    COMD(REG_CX),
    COMD(REG_DX),
};

#undef COMD
#endif

typedef struct Values {
    vtype val;
    regn  reg;
    int    ip;
} val_t;

err_t FuncFinder(char *word, com_t *cmd);
void ParseLine(buf_t *Buf, com_t *cmd, val_t *vals, mk_t *Marks);
void get_word(char *word, size_t *wrdptr, buf_t *Buf);
void check_for_mem(char *word, size_t *wrdptr, com_t *cmd);
err_t get_vals(char* word, buf_t *Buf, com_t *cmd, val_t *vals, mk_t *marks);
err_t get_reg_num(char *name, regn *num);
void write_cmd(buf_t *Buf, com_t *cmd, val_t *vals);


