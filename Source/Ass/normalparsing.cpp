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

int check_for_mark(char *word, size_t *wrdlen);
void add_mark(char *word, size_t wordlen, mk_t *marks, size_t ip);
size_t find_mark(char *word, mk_t *marks);
void AssInit(Conditions *Cons, char **buffer, size_t *len);

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
err_t Assembler (char **buffer, size_t *len, char **output, size_t *outlen){

    assert (buffer  != NULL);
    assert (*(buffer)  != NULL);

    Conditions AssemblerConditions = {};
    AssInit(&AssemblerConditions, buffer, len);

    for (int bypass = 0; bypass < 2; bypass++){
        AssemblerConditions.Buffer.rdr = 1;
        AssemblerConditions.Buffer.wrt = 0;
        while (AssemblerConditions.Buffer.rdr < *len) {

            AssemblerConditions.CommandValues = {};
            AssemblerConditions.CurrentCommand = {};

            ParseLine(&AssemblerConditions);
            AssemblerConditions.Buffer.rdr++;

            if (AssemblerConditions.CurrentCommand.cmd)
                write_cmd(&AssemblerConditions);
        }
    }

    AssemblerConditions.Buffer.wbuf = (char*) realloc (AssemblerConditions.Buffer.wbuf,
                                                       AssemblerConditions.Buffer.wrt*sizeof(char));
    assert(AssemblerConditions.Buffer.wbuf != NULL);

    *output = AssemblerConditions.Buffer.wbuf;
    *outlen = AssemblerConditions.Buffer.wrt;

    return CAT_CONDITION;

}

void AssInit(Conditions *Cons, char **buffer, size_t *len){

    *Cons = {
        .Buffer = {
            .buffer = *(buffer),
            .wbuf = (char*) calloc ((*len) * 2, sizeof(char)),
            .rdr = 1, //since buffer starts with \0
            .wrt = 0,
        },

        .Marks = {
            .mark_array = (mark_t*) calloc (nmarks, sizeof(mark_t)),
            .size = nmarks,
            .ptr = 0,
        },

        .CurrentCommand = {},

        .CommandValues = {},
    };
}

#define readval  Buf->buffer[Buf->rdr]
#define writeval Conditions->Buffer.wbuf[Conditions->Buffer.wrt]

void ParseLine(Conditions *Conditions){

    char *word = (char *) calloc (MaxCmdSize, sizeof(char));
    size_t wordlen = 0;
    size_t wrdcnt  = 0;

    while (Conditions->Buffer.buffer[Conditions->Buffer.rdr] != '\n'){

        get_word(word, &wordlen, &Conditions->Buffer);
        wrdcnt++;

        if (wrdcnt == 1){                      // Cmd or Mark Case

            if (check_for_mark(word, &wordlen)) {

                add_mark(word, wordlen, &Conditions->Marks, Conditions->Buffer.wrt);
                break;
            }

            FuncFinder(word, &Conditions->CurrentCommand);
        }
        else {
            check_for_mem(word, &wordlen, &Conditions->CurrentCommand);
            get_vals(word, wordlen, Conditions);
        }

        if (Conditions->Buffer.buffer[Conditions->Buffer.rdr] == ';' ||
            Conditions->Buffer.buffer[Conditions->Buffer.rdr] == '\r')    //strip comment
            while (Conditions->Buffer.buffer[Conditions->Buffer.rdr] != '\n') Conditions->Buffer.rdr++;

        memset(word, '\0', MaxCmdSize);
        wordlen = 0;
    }

    free(word);
}

void add_mark(char *word, size_t wordlen, mk_t *marks, size_t ip){

    bool wasmark = find_mark(word, marks);

    if (!wasmark) {

        for (size_t i = 0; i< wordlen;i++)
            marks->mark_array[marks->ptr].name[i] = word[i];

        marks->mark_array[marks->ptr].ptr = ip;
        marks->ptr++;
    }

}

size_t find_mark(char *word, mk_t *marks){
    size_t ip = 0;

    for (size_t i = 0; i < marks->ptr &&marks->ptr > 0;i++){

        if(!strcmp(word, marks->mark_array[i].name)){

            ip = marks->mark_array[i].ptr;
        }

    }
    return ip;

}

int check_for_mark(char *word, size_t *wrdlen){

    if (word[--(*wrdlen)] == ':') {

        word[*wrdlen] = '\0';
        return true;
    }
    return false;
}

void write_cmd(Conditions *Conditions){

    memcpy(&writeval, &Conditions->CurrentCommand, cmdsize);
    Conditions->Buffer.wrt += cmdsize;

    if (Conditions->CurrentCommand.ico && !Conditions->CommandValues.is_int) {

        memcpy(&writeval, &Conditions->CommandValues.val, vsize);
        Conditions->Buffer.wrt += vsize;
    }
    else if (Conditions->CurrentCommand.reg && Conditions->CommandValues.reg) {

        memcpy(&writeval, &Conditions->CommandValues.reg, regsize);
        Conditions->Buffer.wrt += regsize;
    }
    else if (Conditions->CurrentCommand.ico){

        memcpy(&writeval, &Conditions->CommandValues.ip, sizeof(int));
        Conditions->Buffer.wrt += sizeof(int);
    }
}

void get_word(char *word, size_t *wrdlen, buf_t *Buf){

    for (;readval == ' '; Buf->rdr++); //strip

    char c = (readval == '[') ? ']' : ' ';

    while (readval != '\r' &&
           readval != c  &&
           readval != EOF  &&
           readval != ';'   ){

        word[(*wrdlen)++] = readval;
        Buf->rdr++;
    }

    if (readval == ']'){
        word[(*wrdlen)++] = readval;
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
    }

    if (!wasword) {printf("%s", word); return COMANDA_KRINGE;}

    return CAT_CONDITION;
}

void check_for_mem(char *word, size_t *wrdlen, com_t *cmd){

    size_t wordlen = *wrdlen;

    if (word[0] == '[' && word[wordlen - 1] == ']'){

        memmove(word, word + 1, wordlen - 2);
        word[--wordlen] = '\0';
        word[--wordlen] = '\0';
        cmd->mem = 1;
    }

    *wrdlen = wordlen;
}

err_t get_vals(char* word, size_t wordlen, Conditions *Conditions){


    if (check_for_plus(word, wordlen)){
        parse_plus_str(word, wordlen, Conditions);
    }

    else if (isdigit(word[0]) || word[0] == '-'){
        parse_constant(word, Conditions);
    }

    else {
        parse_word(word, Conditions);
    }

    return CAT_CONDITION;
}

void parse_word(char *word, Conditions *Conditions){
    regn regn = NON_REG;
    if (get_reg_num(word, &regn)){

        Conditions->CommandValues.reg = regn;
        Conditions->CurrentCommand.reg = 1;
    }
    else {

        Conditions->CommandValues.ip = find_mark(word, &Conditions->Marks);
        Conditions->CurrentCommand.ico = 1;
        Conditions->CommandValues.is_int = true;
    }
}

void parse_constant(char *word, Conditions *Conditions){

    if(needs_ip(Conditions->CurrentCommand)){

        int ip = 0;
        fromstrtoint(word, &ip);

        Conditions->CommandValues.ip = ip;
        Conditions->CommandValues.is_int = true;
        Conditions->CurrentCommand.ico = 1;
    }
    else{
        vtype value = 0;

        castfromstr(word, &value);
        Conditions->CommandValues.val = value;
        Conditions->CurrentCommand.ico = 1;
    }
}

int needs_ip(com_t cmd){

    bool gets_int = true;

    for (int i = 0; i < ncmds; i++){
        if (FindFunc[i].command == cmd.cmd &&
            FindFunc[i].gets_vtype){

                gets_int = false;
        }
    }
    return gets_int;
}

bool check_for_plus(char *word, size_t len){

    for (size_t i = 0; i < len; i++){
        if (word[i] == '+') return true;
    }

    return false;
}

void parse_plus_str(char *word, size_t len, Conditions *Conditions){

    char *word1 = word;
    char *word2 = NULL;

    split_by_plus(word, len, &word2);

    regn regn = NON_REG;

    Conditions->CurrentCommand.ico = 1;
    Conditions->CurrentCommand.reg = 1;

    if (get_reg_num(word1, &regn)){
        Conditions->CommandValues.reg = regn;

        vtype value = 0;
        castfromstr(word2, &value);
        Conditions->CommandValues.val = value;
    }
    else if (get_reg_num(word2, &regn)){
        Conditions->CommandValues.reg = regn;

        vtype value = 0;
        castfromstr(word1, &value);
        Conditions->CommandValues.val = value;
    }
    else
        printf("KOLYA KRINJANUL");
}

void split_by_plus(char *word, size_t len, char **outword){

    for (size_t i = 0; i < len; i++){
        if (word[i] == '+'){

            word[i] = '\0';
            *outword = word + i + 1;
        }
    }
}

int get_reg_num(char *name, regn *num){

    bool wasreg = false;

    for (int i = 0; i < nreg; i++){
        if (!stricmp(name, RegArray[i].name)){

            *num = RegArray[i].value;
            wasreg = true;
        }
    }

    return wasreg;
}

const char *Skip4el(const char* CmdName){
    return CmdName + LenOfCmdPhrase;
}
void AsmSetCastFunc(int (*castfunc) (char *str, vtype *val)){
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

int fromstrtoint(char *str, int *val){
    return sscanf(str, "%d", val);
}
