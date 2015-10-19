#ifndef DIMS_S
#define DIMS_S

#include "Pos.h"
    
class Dims 
{
public:
    const unsigned int w, h, d;
    Dims(unsigned int w, unsigned int h, unsigned int d) 
    : w(w), h(h), d(d)
    {
    }
    bool operator==(const Dims& p) const { return w==p.w&&h==p.h&&d==p.d; }
    bool operator!=(const Dims& p) const { return w!=p.w||h!=p.h||d!=p.d; }
    struct iterator : public Pos
    {
        Dims& dims;
        
        iterator(unsigned int x, unsigned int y, unsigned int z, Dims& dims) 
        : Pos(x, y, z)
        , dims(dims) 
        { }
        
        iterator& operator++() 
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
        iterator operator++(int) { iterator ret = *this; ++(*this); return ret; }
        Pos& operator*() { return *this; }
        Pos* operator->() { return &*(*this); }
        bool operator==(iterator other) { return &dims == &other.dims && x == other.x && y == other.y && z == other.z; }
        bool operator!=(iterator other) { return !(*this == other); }
    };
    iterator end() { return iterator(0,0,d, *this); }
    iterator begin() { return iterator(0,0,0, *this); }
};
class Dims2
{
public:
    const unsigned int w, h;
    Dims2(unsigned int w, unsigned int h)
    : w(w), h(h)
    {
    }
    bool operator==(const Dims2& p) const { return w==p.w&&h==p.h; }
    bool operator!=(const Dims2& p) const { return w!=p.w||h!=p.h; }
    struct iterator : public Pos
    {
        Dims2& dims;
        
        iterator(unsigned int x, unsigned int y, Dims2& dims) 
        : Pos(x, y, 0)
        , dims(dims) 
        { }
        
        iterator& operator++() 
        {
            x++;
            if (x == dims.w) 
            {
                x = 0;
                y++;
            }
            return *this;
        }
        iterator operator++(int) { iterator ret = *this; ++(*this); return ret; }
        Pos& operator*() { return *this; }
        Pos* operator->() { return &*(*this); }
        bool operator==(iterator other) { return &dims == &other.dims && x == other.x && y == other.y && z == other.z; }
        bool operator!=(iterator other) { return !(*this == other); }
    };
    iterator end() { return iterator(0,h, *this); }
    iterator begin() { return iterator(0,0, *this); }
};



Pos operator *(Pos a, Dims2 b)
{
    return Pos(a.x * b.w, a.y * b.h, a.z);
}

Dims operator +(Dims a, Dims2 b)
{
    return Dims(a.w + b.w, a.h + b.h, a.d);
}





#endif // DIMS_S