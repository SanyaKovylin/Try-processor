#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/stat.h>

#include "commands.h"
#include "runheader.h"

template <typename T>
inline T val (T value) { return StackCheck(value);}

#define val(value) ( value )
#define verifiedby(what) || val(what)
#define IS_CMD(cmd) (stricmp(command, SkipCmd(#cmd)) == 0)

int to_int(char *val, int *elem);
int to_d(char *val, double *elem);
size_t Read (const char *src, char **Buffer);
err_t ParseOneLine(char *buf, size_t *rd, char *order, size_t *ordptr);
int IsZero(double el);

#define INIT_EL1_EL2_ANS vtype el1 = (vtype) DefaultTypeValue;\
    vtype el2 = (vtype) DefaultTypeValue;\
    vtype ans = (vtype) DefaultTypeValue;\
    GetTwoValues(maincons->Buffer, &el1, &el2);

#define INIT_EL1_EL2 vtype el1 = (vtype) DefaultTypeValue;\
    vtype el2 = (vtype) DefaultTypeValue;\
    GetTwoValues(maincons->Buffer, &el1, &el2);

#define PUSH_ANS(ans)  StackPush(maincons->Buffer, &ans) verifiedby(maincons->Buffer);

#define GET_ELEM vtype elem = DefaultTypeValue;\
    StackPop (maincons->Buffer, &elem) verifiedby(maincons->Buffer);

#define GET_VAL vtype val = 0;\
    memcpy(&val, maincons->Ips + maincons->ip, vsize);\
    maincons->ip += vsize;

void Run(const char* src){
    Stack stack = {};
    StackCtor(&stack, DefaultSize, sizeof(vtype), 'p', print_int);

    cond_t RunConditions {
            .Buffer = &stack,
            .Regs = RegArray,
            .Ips = NULL,
            .ip = 0,
            .decodefunc = castfromstr,
            .tocontinue = true,
            .ipm = 0,
        };

    size_t nip = 0;


    Parser(src, &RunConditions.Ips, &nip);

    RunConditions.ipm = nip;
    while (RunConditions.tocontinue){

        cmd_t cmd = (cmd_t) RunConditions.Ips[RunConditions.ip];
        RunConditions.ip += cmdsize;

        for (int i = 0; i < ncmds; i++){
            if (cmd == GetFunc[i].command){
                GetFunc[i].cmdfunc(&RunConditions);
            }
        }
        DUMP(&stack);
    }
}

void Parser (const char *src, char **Ips, size_t *ordn){

    char *Buffer = NULL;
    size_t len = Read(src, &Buffer);

    size_t reader = 1;
    size_t lloc = 16;
    char *ips = (char*) calloc (lloc, sizeof(char));
    size_t nip = 0;

    while (reader < len) {

        if (lloc < nip + cmdsize + vsize) {
            ips = (char*) realloc (ips, lloc *= scale);

            assert(ips != NULL);
        }

        ParseOneLine(Buffer, &reader, ips, &nip);
        reader++;
    }

    // for (int i = 0; i < nip; i++){
    //     printf("%d\n", ips[i]);
    // }

    *(ordn) = nip;
    *(Ips) = ips;
}

err_t ParseOneLine(char *buf, size_t *rd, char *ips, size_t *iptr){

    size_t ipptr = *(iptr);
    size_t reader = *rd;
    char cmd = (buf[reader] - 48);
    memcpy(ips + ipptr, &cmd, cmdsize);

    ipptr += cmdsize;
    reader++;

    char *word = (char *) calloc (MaxCmdSize, sizeof(char));
    size_t wordptr = 0;

    while (buf[reader] != '\n'){

        while (buf[reader] != ' '  &&
               buf[reader] != '\r' &&
               buf[reader] != EOF  ){

            word[wordptr++] = buf[reader++];
        }

        if (wordptr > 0){

            if (cmd == CMD_PUSHR || cmd == CMD_POP){

                if (sizeof(word) != RegLen){
                    return SAM_PRIDUMAL_REG_TAKOY;
                }

                memcpy(ips + ipptr, word, RegLen);
                ipptr += RegLen;
            }

            vtype val = 0;
            castfromstr(word, &val);
            // printf("{%s} [%f] (%lld)\n", word, val, ipptr);
            memcpy(ips + ipptr, &val, vsize);
            ipptr += vsize;

            memset(word,'\0', MaxCmdSize);
            wordptr = 0;
        }

        while (buf[reader] == ' ' || buf[reader] == '\r') {

            reader++;
        }
    }

    *(iptr) = ipptr;
    *(rd) = reader;
    return CAT_CONDITION;
}

size_t Read (const char *src, char **Buffer) {

    assert (src    != NULL);
    assert (Buffer != NULL);

    int fo = open (src, O_RDONLY | O_BINARY);

    struct stat st = {};

    stat (src, &st);
    _off_t readlen = st.st_size;

    *Buffer = (char*) calloc (readlen + 2, sizeof (char));
    *Buffer[0] = '\0';

    size_t lenbuf = read (fo, *Buffer + 1, readlen);
    return lenbuf;
}

int to_int(char *val, int *elem){
    char* end;
    *elem = (int) strtod(val, &end);
    return 0;
}

int to_d(char *val, double *elem){
    char* end;
    *elem = strtod(val, &end);
    return 0;
}

void SetCastFunc(int (*castfunc) (char *str, vtype *val)){
    castfromstr = castfunc;
}

const double eps = 10e-6;
int IsZero(double el){
    return abs(el) < eps;
}

static err_t GetTwoValues(Stack *src, vtype *elem1, vtype *elem2){

    StackPop(src, elem1) verifiedby(src);
    StackPop(src, elem2) verifiedby(src);
    return CAT_CONDITION;
}



//=================================================================================
//TODO: Checker!!!
static err_t func_push(cond_t *maincons){

    GET_VAL;
    FUNC_PRINT(val);
    StackPush(maincons->Buffer, &val) verifiedby(maincons->Buffer);
    return CAT_CONDITION;
}

static err_t func_out(cond_t *maincons){

    vtype topelem = (vtype) DefaultTypeValue;
    StackPop(maincons->Buffer, &topelem) verifiedby(maincons->Buffer);

    printf("YOUR RES :: %g", topelem);

    return CAT_CONDITION;
}

static err_t func_in(cond_t *maincons){

    static char *cmd = (char*) calloc (MaxCmdSize, sizeof(char));
    scanf("%s", cmd);

    vtype elem = (vtype) DefaultTypeValue;
    maincons->decodefunc(cmd, &elem) verifiedby(maincons->Buffer);

    return (err_t) (StackPush(maincons->Buffer, &elem));
}

static err_t func_dump(cond_t *maincons){

    StackDump(maincons->Buffer, "DefaultDump", 0, "DefaultDump");
    return CAT_CONDITION;
}

static err_t func_halt(cond_t *maincons){

    maincons->tocontinue = false;
    return CAT_CONDITION;
}

static err_t func_add(cond_t *maincons){

    INIT_EL1_EL2_ANS;
    ans = el1 + el2;
    PUSH_ANS(ans);

    return CAT_CONDITION;
}

static err_t func_sub(cond_t *maincons){

    INIT_EL1_EL2_ANS;
    ans = el2 - el1;
    PUSH_ANS(ans);

    return CAT_CONDITION;
}

static err_t func_mul(cond_t *maincons){

    INIT_EL1_EL2_ANS;
    ans = el1 * el2;
    PUSH_ANS(ans);

    return CAT_CONDITION;
}

static err_t func_div(cond_t *maincons){

    INIT_EL1_EL2_ANS;

    if (IsZero(el2)){
        printf("division by zero");
        return NE_PODELISH_NA_NICHTO;
    }

    ans = el2 / el1;
    PUSH_ANS(ans);

    return CAT_CONDITION;
}

static err_t func_sqrt(cond_t *maincons){

    GET_ELEM;
    elem = sqrt(elem);

    PUSH_ANS(elem);

    return CAT_CONDITION;
}

static err_t func_sin(cond_t *maincons){

    GET_ELEM;
    elem = sin(elem);
    PUSH_ANS(elem);

    return CAT_CONDITION;
}

static err_t func_cos(cond_t *maincons){

    GET_ELEM;
    elem = cos(elem);
    PUSH_ANS(elem);
    return CAT_CONDITION;
}

static err_t func_jmp(cond_t *maincons){

    GET_VAL;
    FUNC_PRINT(val);

    maincons->ip = (int) (val);
    return CAT_CONDITION;
}

static err_t func_ja(cond_t *maincons){
    INIT_EL1_EL2;
    GET_VAL;
    if (el1 > el2){
        maincons->ip = (int) val;
    }
    return CAT_CONDITION;
}

static err_t func_jae(cond_t *maincons){
    INIT_EL1_EL2;
    GET_VAL;
    if (el1 >= el2){
        maincons->ip = (int) val;
    }
    return CAT_CONDITION;
}

static err_t func_jb(cond_t *maincons){
    INIT_EL1_EL2;
    GET_VAL;
    if (el1 < el2){
        maincons->ip = (int) val;
    }

    return CAT_CONDITION;
}

static err_t func_jbe(cond_t *maincons){
    INIT_EL1_EL2;
    GET_VAL;
    if (el1 <= el2){
        maincons->ip = (int) val;
    }
    return CAT_CONDITION;
}

static err_t func_je(cond_t *maincons){
    INIT_EL1_EL2;
    GET_VAL;
    if (IsZero(el1 - el2)){
        maincons->ip = (int) val;
    }
    return CAT_CONDITION;
}

static err_t func_jne(cond_t *maincons){
    INIT_EL1_EL2;
    GET_VAL;
    if (!IsZero(el1 - el2)){
        maincons->ip = (int) val;
    }
    return CAT_CONDITION;
}

#pragma GCC diagnostic ignored "-Wunused-parameter"

static err_t func_call(cond_t *maincons){

    return CAT_CONDITION;
}

static err_t func_ret(cond_t *maincons){

    return CAT_CONDITION;
}

static err_t func_pushr(cond_t *maincons){

    char name[RegLen] = {};
    memcpy(name, maincons->Ips + maincons->ip, RegLen);

    for (int i = 0; i < nreg; i++){
        if (!strcmp(maincons->Regs[i].name, name)){
            StackPush(maincons->Buffer, &maincons->Regs[i].value) verifiedby(maincons->Buffer);
        }
    }

    return CAT_CONDITION;
}

static err_t func_pop(cond_t *maincons){

    char name[RegLen] = {};
    memcpy(name, maincons->Ips + maincons->ip, RegLen);

    for (int i = 0; i < nreg; i++){
        if (!strcmp(maincons->Regs[i].name, name)){
            vtype val = 0;
            StackPop(maincons->Buffer, &val) verifiedby(maincons->Buffer);
            maincons->Regs[i].value = val;
        }
    }
    return CAT_CONDITION;
}

#pragma GCC diagnostic error "-Wunused-parameter"
