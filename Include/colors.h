#ifndef COLORS_H_INCLUDED
#define COLORS_H_INCLUDED

#include <windows.h>

enum Colour {
    Red       =        12,
    LightRed  =         4,
    LightBlue =        11,
    LightGrey =         7,
    Green     =         2,
    DefaultColor = LightGrey,
};

void ChangeColourTo (Colour colour);
#endif // COLORS_H_INCLUDED
