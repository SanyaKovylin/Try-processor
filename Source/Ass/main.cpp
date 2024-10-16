#include <stdio.h>
#include <ctype.h>
#include "assrunheader.h"

void print_int(FILE *fw, void* elem);

const char *src = "Source/cmds.txt";
const char *dst = "Source/SPU.asm";

int fromstrtoint(char *ctr, int *val);
int fromstrtod(char *str, vtype *val);

int main(void){

    char *buffer = NULL;
    size_t lenbuf = BaseRead (src, &buffer);
    SetCastFunc(fromstrtod);

    RIZE(Parser(&buffer, &lenbuf));

    FILE* fdst = fopen(dst, "w");

    printf("!!!%s\n", buffer + 1);

    fputs(buffer + 1, fdst);

    return 0;
}

int fromstrtoint(char *str, int *val){
    return sscanf(str, "%d", val);
}

int fromstrtod(char *str, double *val){
    return ((*(val) = strtod(str, NULL))> 0);
}
