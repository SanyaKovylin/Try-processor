#include <stdio.h>
#include <ctype.h>
#include "assrunheader.h"

void print_int(FILE *fw, void* elem);
char *to_dst(char *word, size_t len);

const char* src = "Source/Fact.txt";
const char *dst = "Source/SPU.asm";

int fromstrtoint(char *ctr, int *val);
int fromstrtod(char *str, vtype *val);

int main(const int argc, const char *argv[]){

    size_t lenbuf = 0;
    char* buffer = NULL;
    if (argc > 1)
        lenbuf = BaseRead (argv[1], &buffer);
    else
        lenbuf = BaseRead (src, &buffer);

    SetCastFunc(fromstrtod);

    RIZE(NParser(&buffer, &lenbuf));
    RIZE(NParser(&buffer, &lenbuf));

    FILE* fdst = fopen(dst, "wb");
    fwrite(buffer, sizeof(char), lenbuf, fdst);
    return 0;
}

int fromstrtoint(char *str, int *val){
    return sscanf(str, "%d", val);
}

int fromstrtod(char *str, double *val){
    return ((*(val) = strtod(str, NULL))> 0);
}

