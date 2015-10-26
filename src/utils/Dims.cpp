#include "Dims.h"

Dims::iterator& Dims::iterator::operator++() 
{
    x++;
    if (x == dims.w) 
    {
        x = 0;
        y++;
        if (y == dims.h)
        {
            y = 0;
            z++;
        }
    }
    return *this;
}

Dims2::iterator& Dims2::iterator::operator++() 
{
    x++;
    if (x == dims.w) 
    {
        x = 0;
        y++;
    }
    return *this;
}

Pos operator *(Pos a, Dims2 b)
{
    return Pos(a.x * b.w, a.y * b.h, a.z);
}

Dims operator +(Dims a, Dims2 b)
{
    return Dims(a.w + b.w, a.h + b.h, a.d);
}

