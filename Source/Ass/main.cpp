#include <stdio.h>
#include <ctype.h>
#include "assrunheader.h"

void print_int(FILE *fw, void* elem);
int fromstrtod(char *str, vtype *val);

const char *SourceFile     = "Source/Progs/Bad Apple.txt";
const char *DetinationFile = "Source/Asm/Bad Apple.asm";

int main(void){

    size_t lenbuf = 0;
    char* buffer = NULL;

    size_t lenout = 0;
    char* outbuf = NULL;

    lenbuf = Read(SourceFile, &buffer);

    AsmSetCastFunc(fromstrtod);

    RIZE(Assembler(&buffer, &lenbuf, &outbuf, &lenout));

    FILE* fdst = fopen(DetinationFile, "wb");
    fwrite(outbuf, sizeof(char), lenout, fdst);

    return 0;
}

int fromstrtod(char *str, double *val){
    return ((*(val) = strtod(str, NULL))> 0);
}

