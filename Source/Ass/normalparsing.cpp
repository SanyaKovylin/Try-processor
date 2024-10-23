#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <sys/stat.h>

#include "nparser.h"
#include "assrunheader.h"

int IsZero(double el);
void pr_bin(void* el, int size);
int check_for_mark(char *word, size_t *wrdptr);
void add_mark(char *word, size_t wordptr, mk_t *marks, size_t ip);
size_t find_mark(char *word, mk_t *marks);

size_t BaseRead (const char *src, char **Buffer) {

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

err_t NParser (char **buffer, size_t *len){
    printf("Parsing...\n");
    assert (buffer  != NULL);
    assert (*(buffer)  != NULL);

    static int count = 0;
    count++;

    buf_t Buf = {
        .buffer = *(buffer),
        .wbuf = (char*) calloc ((*len) * 2, sizeof(char)),
        .rdr = 1, //since buffer starts with \0
        .wrt = 0,
    };

    static mk_t Marks = {
        .marks = (mark_t*) calloc (nmarks, sizeof(mark_t)),
        .size = nmarks,
        .ptr = 0,
    };

    while (Buf.rdr < *len) {

        val_t vals = {};
        com_t cmd = {};

        ParseLine(&Buf, &cmd, &vals, &Marks);
        Buf.rdr++;

        if (cmd.cmd)
            write_cmd(&Buf, &cmd, &vals);
    }

    if (Buf.wrt != 0){
        Buf.wbuf = (char*) realloc (Buf.wbuf, Buf.wrt*sizeof(char));
        assert(Buf.wbuf != NULL);
        if (count == 2)
            *buffer = Buf.wbuf;
    }

    return CAT_CONDITION;

}

#define readval  Buf->buffer[Buf->rdr]
#define writeval Buf->wbuf[Buf->wrt]


void ParseLine(buf_t *Buf, com_t *cmd, val_t *vals, mk_t *Marks){

    char *word = (char *) calloc (MaxCmdSize, sizeof(char));
    size_t wordptr = 0;
    size_t wrdcnt  = 0;

    while (readval != '\n'){

        get_word(word, &wordptr, Buf);
        wrdcnt++;
        // printf("{%s}\n", word);

        if (wrdcnt == 1){ // Cmd or Mark Case
            if (check_for_mark(word, &wordptr)) {
                add_mark(word, wordptr, Marks, Buf->wrt);
                break;
            }
            FuncFinder(word, cmd);

        }
        else {
            check_for_mem(word, &wordptr, cmd);
            get_vals(word, Buf, cmd, vals, Marks);
        }


        if (readval == ';' || readval == '\r')
            while (readval != '\n') Buf->rdr++;

        memset(word, '\0', MaxCmdSize);
        wordptr = 0;
    }

    //TODO: Checkvals

}

void add_mark(char *word, size_t wordptr, mk_t *marks, size_t ip){

    bool wasmark = find_mark(word, marks);
    // printf(" MARK _ %d\n", ip);

    if (!wasmark) {

        for (size_t i = 0; i< wordptr;i++)
            marks->marks[marks->ptr].name[i] = word[i];

        marks->marks[marks->ptr].ptr = ip;
        marks->ptr++;
    }

}

size_t find_mark(char *word, mk_t *marks){
    size_t ip = 0;

    for (size_t i = 0; i < marks->ptr &&marks->ptr > 0;i++){
        // printf("HERE....%s<<%s<", word, marks->marks[i].name);
        if(!strcmp(word, marks->marks[i].name)){
            ip = marks->marks[i].ptr;

        }
    }
    return ip;
}

int check_for_mark(char *word, size_t *wrdptr){
    // printf("{%c}",word[(*wrdptr) - 1]);
    if (word[--(*wrdptr)] == ':') {
        word[*wrdptr] = '\0';
        return true;
    }
    return false;
}

void write_cmd(buf_t *Buf, com_t *cmd, val_t *vals){

    memcpy(&writeval, cmd, cmdsize);
    Buf->wrt += cmdsize;

    // printf("  %d   ", Buf->wrt);
    // pr_bin(cmd, 1);
    // putc(' ', stdout);
    // printf("%d\n", vals->ip);

    // printf(" \nbuf = %s\n", Buf->buffer);
    // printf("\n\nv = %d, 1 = %g, 2 = %d, 3 = %d", cmd->cmd, vals->val, vals->reg, vals->ip);

    if (!IsZero(vals->val) || (cmd->cmd == CMD_PUSH && cmd->ico) || (cmd->cmd == CMD_POP && cmd->ico)) {
        memcpy(&writeval, &vals->val, vsize); // value goes always first
        // pr_bin(&vals->val, vsize);
        // printf("    %g", vals->val);
        // putc(' ', stdout);
        Buf->wrt += vsize;
    }

    if (vals->reg) {
        memcpy(&writeval, &vals->reg, regsize);
        // pr_bin(&vals->reg, regsize);
        // putc(' ', stdout);
        Buf->wrt += regsize;
    }

    if (vals->ip || (CMD_JMP<=cmd->cmd && cmd->cmd<= CMD_CALL)) {
        memcpy(&writeval, &vals->ip, sizeof(int));
        Buf->wrt += sizeof(int);
        // pr_bin(&vals->ip, sizeof(int));
        // putc(' ', stdout);
        // printf("IP _ %d", vals->ip);
    }
    // putc('\n', stdout);
}

void get_word(char *word, size_t *wrdptr, buf_t *Buf){

    for (;readval == ' '; Buf->rdr++); //strip

    char c = (readval == '[') ? ']' : ' ';

    while (readval != '\r' &&
           readval != c  &&
           readval != EOF  &&
           readval != ';'   ){

        word[(*wrdptr)++] = readval;
        Buf->rdr++;
    }

    if (readval == ']'){
        word[(*wrdptr)++] = readval;
        Buf->rdr++;
    }
}

err_t FuncFinder(char *word, com_t *cmd){

    bool wasword = false;

    for (int i = 0; i < ncmds; i++){

       if (!stricmp(word, FindFunc[i].cmdname)){

           cmd->cmd = (cmd_t) (FindFunc[i].command);
           wasword = true;
           break;
        }

        // printf("%s\n", FindFunc[i].cmdname);
    }

    if (!wasword) {printf("%s", word); return COMANDA_KRINGE;}

    return CAT_CONDITION;
}

void check_for_mem(char *word, size_t *wrdptr, com_t *cmd){
    size_t wordptr = *wrdptr;
    if (word[0] == '[' && word[wordptr - 1] == ']'){
                memmove(word, word + 1, wordptr - 2);
                word[--wordptr] = '\0';
                word[--wordptr] = '\0';
                cmd->mem = 1;
            }
    *wrdptr = wordptr;
}

err_t get_vals(char* word, buf_t *Buf, com_t *cmd, val_t *vals, mk_t *marks){

    // assert(*vals != NULL);
    assert( vals != NULL);
    assert(  Buf != NULL);

    char reg[3] = {};
    vtype   val = 0;
    regn regnum = NON_REG;
    int jmp = 0;
    char mark[10] = {};

    int ver2 = sscanf_s(word, "%s + %lg", reg, 3 , &val);

    int ver1 = 0;
    if (!isalpha(reg[0]))
        ver1 = sscanf_s(word, "%lg + %s", &val, reg , 3);

    int ver3 = sscanf(word, "%d", &jmp);
    int ver4 = sscanf(word,"%s:", mark);

    if (ver1 == 2 || ver2 == 2){
        cmd->reg = 1;
        cmd->ico = 1;
        vals->val = val;
        get_reg_num(reg, &regnum);
        vals->reg =(regn) regnum;

    }
    else if (ver1 && ((cmd->cmd == CMD_PUSH) || (cmd->cmd = CMD_POP))) {
        cmd->ico = 1;
        vals->val = val;
    }
    else if (ver2 && ((cmd->cmd == CMD_PUSH) || (cmd->cmd = CMD_POP))) {
        cmd->reg = 1;
        get_reg_num(reg, &regnum);
        vals->reg = (regn) regnum;
    }
    else if (ver3) {
        cmd->ico = 1;
        vals->ip = jmp;
    }
    else if (ver4){
        cmd->ico = 1;
        vals->ip = find_mark(word, marks);
    }
    else return THIS_PAR_IS_INVALID;

    return CAT_CONDITION;
}

err_t get_reg_num(char *name, regn *num){

    bool wasreg = false;

    for (int i = 0; i < nreg; i++){
        if (!stricmp(name, RegArray[i].name)){
            *num = RegArray[i].value;
            wasreg = true;
        }
    }

    if (!wasreg) return SAM_PRIDUMAL_REG_TAKOY;

    return CAT_CONDITION;
}

const char *Skip4el(const char* CmdName){
    return CmdName + LenOfCmdPhrase;
}
void SetCastFunc(int (*castfunc) (char *str, vtype *val)){
    castfromstr = castfunc;
}

#undef readval
#undef writeval

const double eps = 10e-6;
int IsZero(double el){
    return abs(el) < eps;
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
