# 0 "Source/example.cpp"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "Source/example.cpp"
# 1 "Include/enum.h" 1




enum CCCCC {
# 1 "Include/gena.h" 1


CMD_PUSH = 1,
# 12 "Include/gena.h"
CMD_OUT = 2,
# 21 "Include/gena.h"
CMD_IN = 3,
# 32 "Include/gena.h"
CMD_DUMP = 4,
# 41 "Include/gena.h"
CMD_HALT = 5,





CMD_ADD = 6,







CMD_SUB = 7,







CMD_MUL = 8,







CMD_DIV = 9,
# 83 "Include/gena.h"
CMD_SQRT = 10,







CMD_SIN = 11,







CMD_COS = 12,







CMD_JMP = 13,







CMD_JA = 14,
# 125 "Include/gena.h"
CMD_JAE = 15,
# 135 "Include/gena.h"
CMD_JB = 16,
# 145 "Include/gena.h"
CMD_JBE = 17,
# 155 "Include/gena.h"
CMD_JE = 18,
# 165 "Include/gena.h"
CMD_JNE = 19,
# 175 "Include/gena.h"
CMD_CALL = 20,







CMD_RET = 21,
# 192 "Include/gena.h"
CMD_POP = 22,
# 201 "Include/gena.h"
CMD_DRAW = 23,
# 7 "Include/enum.h" 2
}
# 2 "Source/example.cpp" 2
# 1 "Include/forstruct.h" 1
struct E {
    cmd_t command;
    err_t (*cmdfunc)(cond_t *MainConditions);
    int gets_int;
};




# 1 "Include/gena.h" 1


static err_t func_PUSH (cond_t *maincons);
# 12 "Include/gena.h"
static err_t func_OUT (cond_t *maincons);
# 21 "Include/gena.h"
static err_t func_IN (cond_t *maincons);
# 32 "Include/gena.h"
static err_t func_DUMP (cond_t *maincons);
# 41 "Include/gena.h"
static err_t func_HALT (cond_t *maincons);





static err_t func_ADD (cond_t *maincons);







static err_t func_SUB (cond_t *maincons);







static err_t func_MUL (cond_t *maincons);







static err_t func_DIV (cond_t *maincons);
# 83 "Include/gena.h"
static err_t func_SQRT (cond_t *maincons);







static err_t func_SIN (cond_t *maincons);







static err_t func_COS (cond_t *maincons);







static err_t func_JMP (cond_t *maincons);







static err_t func_JA (cond_t *maincons);
# 125 "Include/gena.h"
static err_t func_JAE (cond_t *maincons);
# 135 "Include/gena.h"
static err_t func_JB (cond_t *maincons);
# 145 "Include/gena.h"
static err_t func_JBE (cond_t *maincons);
# 155 "Include/gena.h"
static err_t func_JE (cond_t *maincons);
# 165 "Include/gena.h"
static err_t func_JNE (cond_t *maincons);
# 175 "Include/gena.h"
static err_t func_CALL (cond_t *maincons);







static err_t func_RET (cond_t *maincons);
# 192 "Include/gena.h"
static err_t func_POP (cond_t *maincons);
# 201 "Include/gena.h"
static err_t func_DRAW (cond_t *maincons);
# 11 "Include/forstruct.h" 2






struct E EA[] {
# 1 "Include/gena.h" 1


CMD_PUSH , func_PUSH, (1 - 1),
# 12 "Include/gena.h"
CMD_OUT , func_OUT, (1 - 0),
# 21 "Include/gena.h"
CMD_IN , func_IN, (1 - 0),
# 32 "Include/gena.h"
CMD_DUMP , func_DUMP, (1 - 0),
# 41 "Include/gena.h"
CMD_HALT , func_HALT, (1 - 0),





CMD_ADD , func_ADD, (1 - 0),







CMD_SUB , func_SUB, (1 - 0),







CMD_MUL , func_MUL, (1 - 0),







CMD_DIV , func_DIV, (1 - 0),
# 83 "Include/gena.h"
CMD_SQRT , func_SQRT, (1 - 0),







CMD_SIN , func_SIN, (1 - 0),







CMD_COS , func_COS, (1 - 0),







CMD_JMP , func_JMP, (1 - 0),







CMD_JA , func_JA, (1 - 0),
# 125 "Include/gena.h"
CMD_JAE , func_JAE, (1 - 0),
# 135 "Include/gena.h"
CMD_JB , func_JB, (1 - 0),
# 145 "Include/gena.h"
CMD_JBE , func_JBE, (1 - 0),
# 155 "Include/gena.h"
CMD_JE , func_JE, (1 - 0),
# 165 "Include/gena.h"
CMD_JNE , func_JNE, (1 - 0),
# 175 "Include/gena.h"
CMD_CALL , func_CALL, (1 - 0),







CMD_RET , func_RET, (1 - 0),
# 192 "Include/gena.h"
CMD_POP , func_POP, (1 - 1),
# 201 "Include/gena.h"
CMD_DRAW , func_DRAW, (1 - 0),
# 19 "Include/forstruct.h" 2
};
# 3 "Source/example.cpp" 2

template <typename T>
inline T val (T value) {return StackCheck(value);}
# 34 "Source/example.cpp"
# 1 "Include/gena.h" 1


static err_t func_PUSH(cond_t *maincons){ { vtype *cell = NULL; get_arg(maincons, &cell); StackPush(maincons->Buffer, cell) || ( maincons->Buffer ); } }
# 12 "Include/gena.h"
static err_t func_OUT(cond_t *maincons){ { vtype topelem = (vtype) DefaultTypeValue; StackPop(maincons->Buffer, &topelem) || ( maincons->Buffer ); printf("YOUR RES :: %g\n", topelem); } }
# 21 "Include/gena.h"
static err_t func_IN(cond_t *maincons){ { static char *cmd = (char*) calloc (MaxCmdSize, sizeof(char)); scanf("%s", cmd); vtype elem = (vtype) DefaultTypeValue; maincons->decodefunc(cmd, &elem) || ( maincons->Buffer ); StackPush(maincons->Buffer, &elem) || ( maincons->Buffer ); } }
# 32 "Include/gena.h"
static err_t func_DUMP(cond_t *maincons){ { StackDump(maincons->Buffer, "DefaultDump", 0, "DefaultDump"); printf("ax - %g, bx - %g, cx-%g, dx-%g\n", maincons->Regs[0], maincons->Regs[1], maincons->Regs[2], maincons->Regs[3]); } }
# 41 "Include/gena.h"
static err_t func_HALT(cond_t *maincons){ { maincons->tocontinue = false; } }





static err_t func_ADD(cond_t *maincons){ { vtype el1 = (vtype) DefaultTypeValue; vtype el2 = (vtype) DefaultTypeValue; vtype ans = (vtype) DefaultTypeValue; GetTwoValues(maincons->Buffer, &el1, &el2);; ans = el1 + el2; StackPush(maincons->Buffer, &ans) || ( maincons->Buffer );; } }







static err_t func_SUB(cond_t *maincons){ { vtype el1 = (vtype) DefaultTypeValue; vtype el2 = (vtype) DefaultTypeValue; vtype ans = (vtype) DefaultTypeValue; GetTwoValues(maincons->Buffer, &el1, &el2);; ans = el2 - el1; StackPush(maincons->Buffer, &ans) || ( maincons->Buffer );; } }







static err_t func_MUL(cond_t *maincons){ { vtype el1 = (vtype) DefaultTypeValue; vtype el2 = (vtype) DefaultTypeValue; vtype ans = (vtype) DefaultTypeValue; GetTwoValues(maincons->Buffer, &el1, &el2);; ans = el1 * el2; StackPush(maincons->Buffer, &ans) || ( maincons->Buffer );; } }







static err_t func_DIV(cond_t *maincons){ { vtype el1 = (vtype) DefaultTypeValue; vtype el2 = (vtype) DefaultTypeValue; vtype ans = (vtype) DefaultTypeValue; GetTwoValues(maincons->Buffer, &el1, &el2);; if (IsZero(el1)){ printf("division by zero"); return NE_PODELISH_NA_NICHTO; } ans = el2 / el1; StackPush(maincons->Buffer, &ans) || ( maincons->Buffer );; } }
# 83 "Include/gena.h"
static err_t func_SQRT(cond_t *maincons){ { vtype elem = DefaultTypeValue; StackPop (maincons->Buffer, &elem) || ( maincons->Buffer );; elem = sqrt(elem); StackPush(maincons->Buffer, &elem) || ( maincons->Buffer );; } }







static err_t func_SIN(cond_t *maincons){ { vtype elem = DefaultTypeValue; StackPop (maincons->Buffer, &elem) || ( maincons->Buffer );; elem = sin(elem); StackPush(maincons->Buffer, &elem) || ( maincons->Buffer );; } }







static err_t func_COS(cond_t *maincons){ { vtype elem = DefaultTypeValue; StackPop (maincons->Buffer, &elem) || ( maincons->Buffer );; elem = cos(elem); StackPush(maincons->Buffer, &elem) || ( maincons->Buffer );; } }







static err_t func_JMP(cond_t *maincons){ { int val = 0; memcpy(&val, maincons->Ips + maincons->ip, sizeof(int)); maincons->ip += sizeof(int);; maincons->ip = (int) val; } }







static err_t func_JA(cond_t *maincons){ { vtype el1 = (vtype) DefaultTypeValue; vtype el2 = (vtype) DefaultTypeValue; GetTwoValues(maincons->Buffer, &el1, &el2);; int val = 0; memcpy(&val, maincons->Ips + maincons->ip, sizeof(int)); maincons->ip += sizeof(int);; if (el1 > el2){ maincons->ip = (int) val; } } }
# 125 "Include/gena.h"
static err_t func_JAE(cond_t *maincons){ { vtype el1 = (vtype) DefaultTypeValue; vtype el2 = (vtype) DefaultTypeValue; GetTwoValues(maincons->Buffer, &el1, &el2);; int val = 0; memcpy(&val, maincons->Ips + maincons->ip, sizeof(int)); maincons->ip += sizeof(int);; if (el1 >= el2){ maincons->ip = (int) val; } } }
# 135 "Include/gena.h"
static err_t func_JB(cond_t *maincons){ { vtype el1 = (vtype) DefaultTypeValue; vtype el2 = (vtype) DefaultTypeValue; GetTwoValues(maincons->Buffer, &el1, &el2);; int val = 0; memcpy(&val, maincons->Ips + maincons->ip, sizeof(int)); maincons->ip += sizeof(int);; if (el1 < el2){ maincons->ip = (int) val; } } }
# 145 "Include/gena.h"
static err_t func_JBE(cond_t *maincons){ { vtype el1 = (vtype) DefaultTypeValue; vtype el2 = (vtype) DefaultTypeValue; GetTwoValues(maincons->Buffer, &el1, &el2);; int val = 0; memcpy(&val, maincons->Ips + maincons->ip, sizeof(int)); maincons->ip += sizeof(int);; if (el1 <= el2){ maincons->ip = (int) val; } } }
# 155 "Include/gena.h"
static err_t func_JE(cond_t *maincons){ { vtype el1 = (vtype) DefaultTypeValue; vtype el2 = (vtype) DefaultTypeValue; GetTwoValues(maincons->Buffer, &el1, &el2);; int val = 0; memcpy(&val, maincons->Ips + maincons->ip, sizeof(int)); maincons->ip += sizeof(int);; if (IsZero(el1 - el2)){ maincons->ip = (int) val; } } }
# 165 "Include/gena.h"
static err_t func_JNE(cond_t *maincons){ { vtype el1 = (vtype) DefaultTypeValue; vtype el2 = (vtype) DefaultTypeValue; GetTwoValues(maincons->Buffer, &el1, &el2);; int val = 0; memcpy(&val, maincons->Ips + maincons->ip, sizeof(int)); maincons->ip += sizeof(int);; if (!IsZero(el1 - el2)){ maincons->ip = (int) val; } } }
# 175 "Include/gena.h"
static err_t func_CALL(cond_t *maincons){ { int val = 0; memcpy(&val, maincons->Ips + maincons->ip, sizeof(int)); maincons->ip += sizeof(int);; StackPush(maincons->Calls, &maincons->ip) || ( maincons->Calls ); maincons->ip = (int) val; } }







static err_t func_RET(cond_t *maincons){ { int ip = maincons->ip; StackPop(maincons->Calls, &ip) || ( maincons->Calls ); maincons->ip = ip; return CAT_CONDITION; } }
# 192 "Include/gena.h"
static err_t func_POP(cond_t *maincons){ { vtype *cell = NULL; get_arg(maincons, &cell); StackPop(maincons->Buffer, cell) || ( maincons->Buffer ); } }
# 201 "Include/gena.h"
static err_t func_DRAW(cond_t *maincons){ { char *out = (char *) calloc (LenRam + LenRam/RamRow, sizeof(char)); for (int i = 0; i*RamRow < LenRam; i++){ for (int j = 0; j < RamRow; j+=(withcolor+1)){ if (withcolor){ } else{ out[(1+RamRow)*i + j] = maincons->RAM[RamRow*i + j]; } } out[(1+RamRow)*i + RamRow] = '\n'; } puts(out); putc('\n', stdout); putc('\n', stdout); Sleep(32); system("cls"); return CAT_CONDITION; } }
# 35 "Source/example.cpp" 2
