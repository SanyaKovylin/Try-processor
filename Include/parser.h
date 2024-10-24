#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include "cmd.h"

struct CmdName {
    cmd_t command;
    const char *cmdname;
};

const char *SkipCmd(const char* CmdName);

#define COMD(name) {name, SkipCmd(#name)}

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
    COMD(CMD_PUSHR),
    COMD(CMD_POP),
};

#undef COMD

int (*castfromstr) (char *str, vtype* val) = NULL;

typedef struct Mark {
    char *name;
    size_t ptr;
} mark_t;

typedef struct Mk {
    mark_t *marks;
    size_t size;
    size_t ptr;
} mk_t;

const int nmarks = 8;

#endif
