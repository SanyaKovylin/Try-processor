#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

#include "parser.h"
#include "assrunheader.h"

FILE *f = fopen("Buf.log","r");

err_t ParseOneLine(char *buf, size_t *rd, size_t *wrt);
err_t FuncFinder(char *word, char *cmd);

size_t BaseRead (const char *src, char **Buffer) {

    assert (src    != NULL);
    assert (Buffer != NULL);

    int fo = open (src, O_RDONLY | O_BINARY);

    struct stat st = {};

    stat (src, &st);
    _off_t readlen = st.st_size;
    // _off_t readlen = 1500;

    *Buffer = (char*) calloc (readlen + 2, sizeof (char));
    *Buffer[0] = '\0';

    size_t lenbuf = read (fo, *Buffer + 1, readlen);
    return lenbuf;
}


err_t Parser (char **buffer, size_t *len){

    assert (buffer  != NULL);
    assert (*(buffer)  != NULL);

    char *buf = *(buffer);

    valtype *cmds = (valtype*) calloc (MaxCommands, sizeof(valtype));
    assert (cmds != NULL);

    size_t writer = 1;
    size_t reader = 1;

    while (reader < *len){

        RIZE(ParseOneLine(buf, &reader, &writer));
    }

    buf = (char*) realloc (buf, writer + 1);
    assert (buf != NULL);

    buf[writer] = '\0';
    printf("%s", buf);
    *(buffer) = buf;

    return CAT_CONDITION;
}

err_t ParseOneLine(char *buf, size_t *rd, size_t *wrt){

    char *word = (char *) calloc (MaxCmdSize, sizeof(char));
    size_t wordptr = 0;


    size_t reader = *rd;
    size_t writer = *wrt;

    while (buf[reader] != '\r' &&
           buf[reader] != ' '  &&
           buf[reader] != EOF  &&
           buf[reader] != ';'   ){

        word[wordptr++] = buf[reader++];
    }

    if (*(word)) {
        char cmd = NON_CMD;
        RIZE(FuncFinder(word, &cmd));
        buf[writer++] = cmd;
    }

    while (buf[reader] != '\r' &&
           buf[reader] != EOF  &&
           buf[reader] != ';'   ) {

           buf[writer++] = buf[reader++];
    }

    if (buf[reader] == '\r') reader++;

    if (buf[reader] == '\n'){

        buf[writer++] = buf[reader++];
    }

    *(rd) = reader;
    *(wrt) = writer;



    return CAT_CONDITION;
}

err_t FuncFinder(char *word, char *cmd){

    bool wasword = false;

    for (int i = 0; i < ncmds; i++){

       if (!stricmp(word, FindFunc[i].cmdname)){
           *(cmd) = (char) FindFunc[i].command + 48;
           wasword = true;
        }
    }

    if (!wasword) return COMANDA_KRINGE;

    return CAT_CONDITION;
}


const char *SkipCmd(const char* CmdName){
    return CmdName + LenOfCmdPhrase;
}

void SetCastFunc(int (*castfunc) (char *str, vtype *val)){
    castfromstr = castfunc;
}
