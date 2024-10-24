// #include <TXLib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdint.h>
#include <sys/stat.h>
#include <windows.h>
#include <conio.h>

#include "commands.h"
#include "runheader.h"

template <typename T>
inline T val (T value) {return StackCheck(value);}

#define val(value) ( value )
#define verifiedby(what) || val(what)
#define IS_CMD(cmd) (stricmp(command, SkipCmd(#cmd)) == 0)

int to_int(char *val, int *elem);
int to_d(char *val, double *elem);
size_t Read (const char *src, char **Buffer);
int IsZero(double el);
void get_arg(cond_t *cons, vtype **cell);
void pr_bin(void* el, int size);

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

#define GET_VAL int val = 0;\
    memcpy(&val, maincons->Ips + maincons->ip, sizeof(int));\
    maincons->ip += sizeof(int);

FILE* fllog = fopen("Regs.log", "w");

void Run(const char* src){
    Stack stack = {};
    StackCtor(&stack, DefaultSize, sizeof(vtype), 'p', print_d);

    Stack calls = {};
    StackCtor(&calls, DefaultSize, sizeof(int), 'e', print_int);

    cond_t RunConditions {
            .Buffer = &stack,
            .Calls = &calls,
            .RAM =  (vtype*) calloc (LenRam, sizeof(vtype)),
            .Regs = (vtype*) calloc (nreg  , sizeof(vtype)),
            .Ips = NULL,
            .ip = 0,
            .decodefunc = castfromstr,
            .tocontinue = true,
            .cmd = {},
            .ipm = 0,
        };

    size_t nip = Read(src, &RunConditions.Ips);

    // Parser(src, &RunConditions.Ips, &nip);

    RunConditions.ipm = nip;
    while (RunConditions.tocontinue){

        memcpy(&RunConditions.cmd, &RunConditions.Ips[RunConditions.ip], 1);
        RunConditions.ip += cmdsize;

        for (int i = 0; i < ncmds; i++){
            if (RunConditions.cmd.cmd == GetFunc[i].command){
                GetFunc[i].cmdfunc(&RunConditions);
            }
        }
        DUMP(&stack);
    }
}

void Parser (const char *src, char **Ips, size_t *ordn){

    char *Buffer = NULL;
    size_t len = Read(src, &Buffer);

    *(ordn) = len;
    *(Ips) = Buffer;
}

size_t Read (const char *src, char **Buffer) {

    assert (src    != NULL);
    assert (Buffer != NULL);

    int fo = open (src, O_RDONLY | O_BINARY);

    struct stat st = {};

    stat (src, &st);
    _off_t readlen = st.st_size;

    *Buffer = (char*) calloc (readlen + 1, sizeof (char));

    size_t lenbuf = read (fo, *Buffer, readlen);
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

    vtype *cell = NULL;
    get_arg(maincons, &cell);

    StackPush(maincons->Buffer, cell) verifiedby(maincons->Buffer);

    return CAT_CONDITION;
}

static err_t func_out(cond_t *maincons){

    vtype topelem = (vtype) DefaultTypeValue;
    StackPop(maincons->Buffer, &topelem) verifiedby(maincons->Buffer);

    printf("YOUR RES :: %g\n", topelem);

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
    printf("ax - %g, bx - %g, cx-%g, dx-%g\n", maincons->Regs[0], maincons->Regs[1],
                                               maincons->Regs[2], maincons->Regs[3]);
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

    if (IsZero(el1)){
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


static err_t func_call(cond_t *maincons){
    GET_VAL;
    StackPush(maincons->Calls, &maincons->ip) verifiedby(maincons->Calls);
    maincons->ip = (int) val;
    return CAT_CONDITION;
}

static err_t func_ret(cond_t *maincons){

    int ip = maincons->ip;
    StackPop(maincons->Calls, &ip) verifiedby(maincons->Calls);
    maincons->ip = ip;
    return CAT_CONDITION;
}

static err_t func_pop(cond_t *maincons){

    vtype *cell = NULL;
    get_arg(maincons, &cell);

    StackPop(maincons->Buffer, cell) verifiedby(maincons->Buffer);

    return CAT_CONDITION;
}

static err_t func_draw(cond_t *maincons){

    char *out = (char *) calloc (LenRam + LenRam/RamRow, sizeof(char));

    for (int i = 0; i*RamRow < LenRam; i++){
        for (int j = 0; j < RamRow; j+=(withcolor+1)){
            if (withcolor){
                //TODO:color
            }
            else{
                out[(1+RamRow)*i + j] = maincons->RAM[RamRow*i + j];

            }
        }
        out[(1+RamRow)*i + RamRow] = '\n';
        // putc('\n', stdout);
    }
    puts(out);
    putc('\n', stdout);
    putc('\n', stdout);
    Sleep(32);
    system("cls");
    return CAT_CONDITION;
}

static err_t func_redp(cond_t *maincons){

    printf("ax - %g, bx - %g, cx-%g, dx-%g\n",maincons->Regs[0],maincons->Regs[1],
                                                      maincons->Regs[2],maincons->Regs[3]);
    return CAT_CONDITION;
}

void get_arg(cond_t *cons, vtype **cell){

    static vtype value = 0;
    value = 0;
    vtype cnst = 0;
    int jmp = 0;
    int reg = 0;
    com_t cmd = cons->cmd;
    // pr_bin(&cmd,1);
    // putc('\n', stdout);
    if (cmd.ico) {
        if (CMD_JMP <= cmd.cmd && cmd.cmd <= CMD_JNE){
            memcpy(&jmp, cons->Ips + cons->ip, sizeof(int));
            cons->ip += sizeof(int);
            // printf("here1");
            value += cnst;
        }
        else{
            memcpy(&cnst, cons->Ips + cons->ip, sizeof(vtype));
            cons->ip += sizeof(vtype);
            // printf("here");
            value += cnst;
        }
    }
    // printf("\nHERE%d", cons->ip);
    if (cmd.reg) {
        memcpy(&reg, cons->Ips + cons->ip, sizeof(char));
        cons->ip += sizeof(char);
        // printf("%d", reg);
        value += cons->Regs[reg - 1];
    }

    // printf("EEEE%g\n", value);

    if (cmd.mem) {*(cell) = &cons->RAM[(int) value];
    // printf("%d\n", (int) value);
    }
    else if      (cmd.ico) *(cell) = &value;
    else              *(cell) = cons->Regs + reg - 1;
}

void pr_bin(void* el, int size){
    for (int i = 0; i < size; i++){
        int x = 7;
        uint8_t y = *((char*) el + i);
        while (x >= 0){
            printf("%d", (y >> x) & 1);
            x--;
        }
    }
}
