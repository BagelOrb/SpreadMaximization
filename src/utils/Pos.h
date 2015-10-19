#ifndef POS_H
#define POS_H


struct Pos
{
    unsigned int x, y, z;
    Pos(unsigned int x, unsigned int y, unsigned int z) 
    : x(x), y(y), z(z) 
    { }
    
    Pos operator+(const Pos p) const { return Pos(x+p.x, y+p.y, z+p.z); }
    Pos operator-(const Pos p) const { return Pos(x-p.x, y-p.y, z-p.z); }
    
    Pos& operator += (const Pos& p) { x += p.x; y += p.y; z += p.z; return *this; }
    Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; z -= p.z; return *this; }
    
    
    bool operator==(const Pos& p) const { return x==p.x&&y==p.y&&z==p.z; }
    bool operator!=(const Pos& p) const { return x!=p.x||y!=p.y||z!=p.z; }
};






#endif // POS_H