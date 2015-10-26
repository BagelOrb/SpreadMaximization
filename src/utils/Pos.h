#ifndef POS_H
#define POS_H


struct Pos3
{
    unsigned int x, y, z;
    Pos3(unsigned int x, unsigned int y, unsigned int z) 
    : x(x), y(y), z(z) 
    { }
    
    Pos3 operator+(const Pos3 p) const { return Pos3(x+p.x, y+p.y, z+p.z); }
    Pos3 operator-(const Pos3 p) const { return Pos3(x-p.x, y-p.y, z-p.z); }
    
    Pos3& operator += (const Pos3& p) { x += p.x; y += p.y; z += p.z; return *this; }
    Pos3& operator -= (const Pos3& p) { x -= p.x; y -= p.y; z -= p.z; return *this; }
    
    
    bool operator==(const Pos3& p) const { return x==p.x&&y==p.y&&z==p.z; }
    bool operator!=(const Pos3& p) const { return x!=p.x||y!=p.y||z!=p.z; }
};






#endif // POS_H