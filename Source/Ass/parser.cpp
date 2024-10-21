// #include <assert.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include <fcntl.h>
// #include <string.h>
// #include <ctype.h>
// #include <sys/stat.h>
//
// #include "parser.h"
// #include "assrunheader.h"
//
// FILE *f = fopen("Buf.log","r");
//
// err_t ParseOneLine(char *buf, size_t *rd, size_t *wrt, mk_t *marks);
// err_t FuncFinder(char *word, char *cmd);
// int check_for_reg(char* buf, size_t rdr);
//
// size_t BaseRead (const char *src, char **Buffer) {
//
//     assert (src    != NULL);
//     assert (Buffer != NULL);
//
//     int fo = open (src, O_RDONLY | O_BINARY);
//
//     struct stat st = {};
//
//     stat (src, &st);
//     _off_t readlen = st.st_size;
//     // _off_t readlen = 1500;
//
//     *Buffer = (char*) calloc (readlen + 2, sizeof (char));
//     *Buffer[0] = '\0';
//
//     size_t lenbuf = read (fo, *Buffer + 1, readlen);
//     return lenbuf;
// }
//
//
// err_t Parser (char **buffer, size_t *len){
//
//     assert (buffer  != NULL);
//     assert (*(buffer)  != NULL);
//
//     char *buf = *(buffer);
//
//     size_t writer = 1;
//     size_t reader = 1; // since starts with \0
//
//     mk_t Marks = {
//         .marks = (mark_t) calloc (nmarks, sizeof(mark_t)),
//         .size = nmarks,
//         .ptr = 0,
//     };
//
//     while (reader < *len){
//
//         RIZE(ParseOneLine(buf, &reader, &writer, &Marks));
//     }
//
//     buf = (char*) realloc (buf, writer + 1);
//     assert (buf != NULL);
//
//     buf[writer] = '\0';
//     printf("%s", buf);
//     *(buffer) = buf;
//
//     return CAT_CONDITION;
// }
//
// err_t ParseOneLine(char *buf, size_t *rd, size_t *wrt, mk_t *marks){
//
//     char *word = (char *) calloc (MaxCmdSize, sizeof(char));
//     size_t wordptr = 0;
//
//     size_t reader = *rd;
//     size_t writer = *wrt;
//
//     while (buf[reader] != '\r' &&
//            buf[reader] != ' '  &&
//            buf[reader] != EOF  &&
//            buf[reader] != ';'   ){
//
//         word[wordptr++] = buf[reader++];
//     }
//
//     if (*(word)) {
//         if (word[wordptr - 1] == ':'){  //Mark
//             CheckMark(word, wordptr, writer, marks);
//         }
//         char cmd = NON_CMD;
//         RIZE(FuncFinder(word, &cmd));
//         if (cmd - 48 == CMD_PUSH && check_for_reg(buf, reader)) cmd = (char) (CMD_PUSHR + 48);
//         buf[writer++] = cmd;
//     }
//
//     while (buf[reader] != '\r' &&
//            buf[reader] != EOF  &&
//            buf[reader] != ';'   ) {
//
//            buf[writer++] = buf[reader++];
//     }
//
//     if (buf[reader] == '\r') reader++;
//N
//     if (buf[reader] == '\n'){
//
//         buf[writer++] = buf[reader++];
//     }
//
//     *(rd) = reader;
//     *(wrt) = writer;
//
//     return CAT_CONDITION;
// }
//
// err_t FuncFinder(char *word, char *cmd){
//
//     bool wasword = false;
//
//     for (int i = 0; i < ncmds; i++){
//
//        if (!stricmp(word, FindFunc[i].cmdname)){
//
//            *(cmd) = (FindFunc[i].command + 48);
//            wasword = true;
//            break;
//         }
//         printf("%s\n", FindFunc[i].cmdname);
//     }
//
//     if (!wasword) {printf("%s", word); return COMANDA_KRINGE;}
//
//     return CAT_CONDITION;
// }
//
// void CheckMark(char *word, size_t wrdlen, size_t writer, mk_t *marks){
//
//     bool wasmark = false;
//
//     for (size_t i = 0; i < marks->ptr; i++){
//         if (!strcmp(marks->marks[i].name, word)){
//             wasmark = true;
//         }
//     }
//
//     if (!wasmark){
//         if (marks->size == marks->ptr){
//
//             marks->marks = (mark_t) realloc (marks->marks,marks->size*scale);
//             narks->size *= scale;
//
//             assert(marks->marks != NULL);
//         }
//
//         marks->ptr = {word, writer};
//     }
// }
//
//
// const char *Skip4el(const char* CmdName){
//     return CmdName + LenOfCmdPhrase;
// }
//
// void SetCastFunc(int (*castfunc) (char *str, vtype *val)){
//     castfromstr = castfunc;
// }
//
// int check_for_reg(char* buf, size_t rdr){
//
//     while (buf[rdr] == ' ') rdr++;
//     return isalpha(buf[rdr]);
// }
