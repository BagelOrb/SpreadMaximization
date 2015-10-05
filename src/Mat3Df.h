#ifndef MAT3DF_H
#define MAT3DF_H

#include <cassert> // assert
#include "debug.h"

class Mat3Df
{
private:
public:
    float* data;
    unsigned int w, h, d;
    unsigned int size;
    
public:
    struct Pos
    {
        unsigned int x, y, z;
        Pos(unsigned int x, unsigned int y, unsigned int z) : x(x), y(y), z(z) 
        { }
        
        Pos operator+(const Pos p) const { return Pos(x+p.x, y+p.y, z+p.z); }
        Pos operator-(const Pos p) const { return Pos(x-p.x, y-p.y, z-p.z); }
        
        Pos& operator += (const Pos& p) { x += p.x; y += p.y; z += p.z; return *this; }
        Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; z -= p.z; return *this; }
        
        bool operator==(const Pos& p) const { return x==p.x&&y==p.y&&z==p.z; }
        bool operator!=(const Pos& p) const { return x!=p.x||y!=p.y||z!=p.z; }
    };    
    class iterator
    {
    public:
        unsigned int x, y, z;
        Mat3Df& mat;
        iterator(unsigned int x, unsigned int y, unsigned int z, Mat3Df& mat) : x(x), y(y), z(z), mat(mat) { }
        friend class Mat3Df;
        iterator& operator++() 
        {
            x++;
            if (x == mat.w) 
            {
                x = 0;
                y++;
                if (y == mat.h)
                {
                    y = 0;
                    z++;
                }
            }
            return *this;
        }
        iterator operator++(int) { iterator ret = *this; ++(*this); return ret; }
        float& operator*() { return mat.data[mat.w*mat.h* z + mat.w* y + x]; }
        float* operator->() { return &*(*this); }
        Pos operator+(iterator other) { return Pos(x+other.x, y+other.y, z+other.z); }
        Pos getPos() { return Pos(x,y,z); }
        bool operator==(iterator other) { return &mat == &other.mat && x == other.x && y == other.y && z == other.z; }
        bool operator!=(iterator other) { return !(*this == other); }
        
        
    };
    iterator end() { return iterator(0,0,d, *this); }
    iterator begin() { return iterator(0,0,0, *this); }
    
    Mat3Df(unsigned int w, unsigned int h, unsigned int d)
    : w(w), h(h), d(d), size(w*h*d)
    {
        data = new float[w*h*d];
    }
    void clear(float val = 0)
    {
        for (int i = 0; i < w*h*d; i++)
            data[i] = val;
    }
    float get(Pos p) { return get(p.x, p.y, p.z); }
    float get(unsigned int x, unsigned int y, unsigned int z)
    {
        assert(x<w && y<h && z<d);
        return data[w*h* z + w* y + x];
    }
    void set(Pos p, float val) { return set(p.x, p.y, p.z, val); };
    void set(unsigned int x, unsigned int y, unsigned int z, float val)
    {
        data[w*h* z + w* y + x] = val;
    }
    void add(Pos p, float val) { data[w*h* p.z + w* p.y + p.x] += val; };
    void add(unsigned int x, unsigned int y, unsigned int z, float val) { data[w*h* z + w* y + x] += val; };
private:
    
    void _convolute(Mat3Df& kernel, Mat3Df& result, unsigned int z)
    {
        assert(kernel.w <= w && kernel.h <= h && kernel.d <= d);
        for (iterator res_it = result.begin(); res_it != iterator(0,0,1,result); ++res_it)
        {
            *res_it = 0;
            for (iterator k_it = kernel.begin(); k_it != kernel.end(); ++k_it)
            {
                Pos dataPos = res_it + k_it;
                result.add(res_it.getPos(), get(dataPos) * *k_it);
            }
        }
    }
public:
    Mat3Df convolute(Mat3Df kernel)
    {
        Mat3Df result(w - kernel.w + 1, h - kernel.h + 1, 1);
        _convolute(kernel, result, 0);
        return result;
    }
    Mat3Df convolute(Mat3Df& kernel, Mat3Df& result, unsigned int z)
    {
        assert(result.w == w - kernel.w + 1 
            && result.h == h - kernel.h + 1);
        _convolute(kernel, result, z);
        return result;
    }
    
    template <class Function>
    void apply(Function f, Mat3Df& result)
    {
        for (unsigned int rp = 0; rp < result.size; rp++)
        {
            result.data[rp] = f(data[rp]);
        }
    }
    template <class Function>
    Mat3Df apply(Function f)
    {
        Mat3Df result(w, h, d);
        apply(f, result);
        return result;
    }
};


#endif//MAT3DF_H
