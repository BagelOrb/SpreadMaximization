#ifndef DIMS_S
#define DIMS_S

#include "Pos.h"
    
class Dims4 
{
public:
    const unsigned int w, h, d, m;
    Dims4(unsigned int w, unsigned int h, unsigned int d, unsigned int m) 
    : w(w), h(h), d(d), m(m)
    {
    }
    bool operator==(const Dims4& p) const { return w==p.w&&h==p.h&&d==p.d&&m==p.m; }
    bool operator!=(const Dims4& p) const { return !(*this==p); }
    struct iterator : public Pos4
    {
        Dims4& dims;
        
        iterator(unsigned int x, unsigned int y, unsigned int z, unsigned int m, Dims4& dims) 
        : Pos4(x, y, z, m)
        , dims(dims) 
        { }
        
        iterator& operator++();
        
        iterator operator++(int) { iterator ret = *this; ++(*this); return ret; }
        Pos4& operator*() { return *this; }
        Pos4* operator->() { return &*(*this); }
        bool operator==(iterator other) { return &dims == &other.dims && x == other.x && y == other.y && z == other.z && n == other.n; }
        bool operator!=(iterator other) { return !(*this == other); }
    };
    iterator end() { return iterator(0,0,0,m, *this); }
    iterator begin() { return iterator(0,0,0,0, *this); }
    unsigned int size() { return w*h*d*m; }
};    

class Dims3 
{
public:
    unsigned int w, h, d;
    Dims3(unsigned int w, unsigned int h, unsigned int d) 
    : w(w), h(h), d(d)
    {
    }
    bool operator==(const Dims3& p) const { return w==p.w&&h==p.h&&d==p.d; }
    bool operator!=(const Dims3& p) const { return w!=p.w||h!=p.h||d!=p.d; }
    struct iterator : public Pos3
    {
        Dims3& dims;
        
        iterator(unsigned int x, unsigned int y, unsigned int z, Dims3& dims) 
        : Pos3(x, y, z)
        , dims(dims) 
        { }
        
        iterator& operator++();
        
        iterator operator++(int) { iterator ret = *this; ++(*this); return ret; }
        Pos3& operator*() { return *this; }
        Pos3* operator->() { return &*(*this); }
        bool operator==(iterator other) { return &dims == &other.dims && x == other.x && y == other.y && z == other.z; }
        bool operator!=(iterator other) { return !(*this == other); }
    };
    iterator end() { return iterator(0,0,d, *this); }
    iterator begin() { return iterator(0,0,0, *this); }
    unsigned int size() { return w*h*d; }
};
class Dims2
{
public:
    unsigned int w, h;
    Dims2(unsigned int w, unsigned int h)
    : w(w), h(h)
    {
    }
    bool operator==(const Dims2& p) const { return w==p.w&&h==p.h; }
    bool operator!=(const Dims2& p) const { return w!=p.w||h!=p.h; }
    struct iterator : public Pos3
    {
        Dims2& dims;
        
        iterator(unsigned int x, unsigned int y, Dims2& dims) 
        : Pos3(x, y, 0)
        , dims(dims) 
        { }
        
        iterator& operator++();
        
        iterator operator++(int) { iterator ret = *this; ++(*this); return ret; }
        Pos3& operator*() { return *this; }
        Pos3* operator->() { return &*(*this); }
        bool operator==(iterator other) { return &dims == &other.dims && x == other.x && y == other.y && z == other.z; }
        bool operator!=(iterator other) { return !(*this == other); }
    };
    iterator end() { return iterator(0,h, *this); }
    iterator begin() { return iterator(0,0, *this); }
    unsigned int size() { return w*h; }
};



Pos3 operator *(Pos3 a, Dims2 b);

Pos3 operator +(Pos3 a, Pos4 b);
Pos4 operator +(Pos4 a, Pos3 b);

Dims3 operator +(Dims3 a, Dims2 b);





#endif // DIMS_S