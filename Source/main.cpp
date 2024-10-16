#include <stdio.h>
#include <ctype.h>

#include "runheader.h"
#include "stack.h"

void print_int(FILE *fw, void* elem);
int fromstrtod(char *str, vtype *val);

const char* Src = "Source/SPU.asm";
const char* LogName = "SPU.log";

int main(void){

    SetCastFunc(fromstrtod);
    SetLog(LogName);
    Run(Src);
}

void print_int(FILE *fw, void* elem) {
    fprintf(fw,"%d", *((int*) elem));
}

int fromstrtod(char *str, vtype *val){
    return ((*(val) = strtod(str, NULL))> 0);
}
