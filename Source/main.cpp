#include <stdio.h>
#include <ctype.h>

#include "runheader.h"
#include "stack.h"

void print_int(FILE *fw, void* elem);
int fromstrtod(char *str, vtype *val);

const char* Src = "Source/Asm/Bad Apple.asm";
const char* LogName = "SPU.log";

int main(void){

    SetCastFunc(fromstrtod);
    SetLog(LogName);
    Run(Src);
}

void print_int(FILE *fw, void* elem) {
    fprintf(fw,"%d", *((int*) elem));
}

void print_d(FILE *fw, void* elem) {
    fprintf(fw,"%g", *((double*) elem));
}

int fromstrtod(char *str, vtype *val){
    return ((*(val) = strtod(str, NULL))> 0);
}
