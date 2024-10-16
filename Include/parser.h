#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include "cmd.h"

struct CmdName {
    cmd_t command;
    const char *cmdname;
};


const char *SkipCmd(const char* CmdName);

#define COMD(name) {name, SkipCmd(#name)}

struct CmdName FindFunc[] {
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
};

int (*castfromstr) (char *str, valtype* val) = NULL;



#endif
