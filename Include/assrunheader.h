#ifndef ASSRUNHEADER_H_INCLUDED
#define ASSRUNHEADER_H_INCLUDED

#include "cmd.h"

void pr_bin(void* el, int size);

size_t Read (const char *src, char **Buffer);
err_t Assembler (char **buffer, size_t *len, char **output, size_t *outlen);

#define RIZE(func) if (err_t c = func) return c;

void AsmSetCastFunc(int (*castfunc) (char *str, vtype *val));
#endif //ASSRUNHEADER_H_INCLUDED
