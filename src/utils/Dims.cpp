#include "Dims.h"

Dims4::iterator& Dims4::iterator::operator++() 
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
            if (z == dims.d)
            {
                z = 0;
                n++;
            }
        }
    }
    return *this;
}

Dims3::iterator& Dims3::iterator::operator++() 
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

Pos3 operator *(Pos3 a, Dims2 b)
{
    return Pos3(a.x * b.w, a.y * b.h, a.z);
}

Pos3 operator +(Pos3 a, Pos4 b)
{
    return Pos3(a.x+b.x, a.y+b.y, a.z+b.z);
}
Pos4 operator +(Pos4 a, Pos3 b)
{
    return Pos4(a.x+b.x, a.y+b.y, a.z+b.z, a.n);
}

Dims3 operator +(Dims3 a, Dims2 b)
{
    return Dims3(a.w + b.w, a.h + b.h, a.d);
}

