#ifndef N_PARSER_H_INCLUDED
#define N_PARSER_H_INCLUDED

#include "cmd.h"

struct CmdName {
    cmd_t command;
    const char *cmdname;
    const int gets_vtype;
};

const char *Skip4el(const char* CmdName);

#define COMD(name) name, Skip4el(#name)

const struct CmdName FindFunc[] {
    COMD(CMD_PUSH),1,
    COMD(CMD_OUT), 1,
    COMD(CMD_IN),  0,
    COMD(CMD_DUMP),0,
    COMD(CMD_HLT), 0,
    COMD(CMD_ADD), 0,
    COMD(CMD_SUB), 0,
    COMD(CMD_MUL), 0,
    COMD(CMD_DIV), 0,
    COMD(CMD_SQRT),0,
    COMD(CMD_SIN), 0,
    COMD(CMD_COS), 0,
    COMD(CMD_JMP), 0,
    COMD(CMD_JA),  0,
    COMD(CMD_JAE), 0,
    COMD(CMD_JB),  0,
    COMD(CMD_JBE), 0,
    COMD(CMD_JE),  0,
    COMD(CMD_JNE), 0,
    COMD(CMD_CALL),0,
    COMD(CMD_RET), 0,
    COMD(CMD_POP), 0,
    COMD(CMD_DRAW),0,
};

typedef struct Mark {
    char name[10];
    size_t ptr;
} mark_t;

typedef struct Mk {
    mark_t *mark_array;
    size_t size;
    size_t ptr;
} mk_t;

const int nmarks = 8;

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
    bool is_int;
    vtype val;
    regn  reg;
    size_t ip;
} val_t;

struct Conditions {
    buf_t Buffer;
    mk_t Marks;
    com_t CurrentCommand;
    val_t CommandValues;

};

int (*castfromstr) (char *str, vtype *val) = NULL;

err_t FuncFinder(char *word, com_t *cmd);
void ParseLine(Conditions *Conditions);
void get_word(char *word, size_t *wrdptr, buf_t *Buf);
void check_for_mem(char *word, size_t *wrdptr, com_t *cmd);
err_t get_vals(char* word, size_t wordlen, Conditions *Conditions);
int get_reg_num(char *name, regn *num);
void write_cmd(Conditions *Conditions);

void parse_word(char *word, Conditions *Conditions);
void parse_constant(char *word, Conditions *Conditions);
int needs_ip(com_t cmd);
bool check_for_plus(char *word, size_t len);
void parse_plus_str(char *word, size_t len, Conditions *Conditions);
void split_by_plus(char *word, size_t len, char **outword);
int fromstrtoint(char *ctr, int *val);
