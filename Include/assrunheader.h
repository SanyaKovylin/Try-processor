#ifndef ASSRUNHEADER_H_INCLUDED
#define ASSRUNHEADER_H_INCLUDED

#include "cmd.h"



size_t BaseRead (const char *src, char **Buffer);
err_t Parser (char **buffer, size_t *len);

#define RIZE(func) if (err_t c = func) return c;

void SetCastFunc(int (*castfunc) (char *str, valtype *val));
#endif //ASSRUNHEADER_H_INCLUDED
