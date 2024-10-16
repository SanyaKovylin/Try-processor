#include <stdio.h>
#include <ctype.h>
#include "assrunheader.h"

void print_int(FILE *fw, void* elem);

const char *src = "Source/cmds.txt";
const char *dst = "Source/SPU.asm";

int fromstrtoint(char *ctr, valtype *val);

int main(void){

    char *buffer = NULL;
    size_t lenbuf = BaseRead (src, &buffer);
    SetCastFunc(fromstrtoint);

    RIZE(Parser(&buffer, &lenbuf));

    FILE* fdst = fopen(dst, "w");

    printf("!!!%s\n", buffer + 1);

    fputs(buffer + 1, fdst);
}

int fromstrtoint(char *str, valtype *val){
    return sscanf(str, "%d", val);
}
