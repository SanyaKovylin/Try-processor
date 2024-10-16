#ifndef RUNHEADER_H_INCLUDED
#define RUNHEADER_H_INCLUDED

#include "cmd.h"

void Run(const char* src);
void Parser (const char *src, char **order ,size_t *ordn);
void SetCastFunc(int (*castfunc) (char *str, vtype *val));

#define RIZE(func) if (err_t c = func) return c;
#endif //RUNHEADER_H_INCLUDED
