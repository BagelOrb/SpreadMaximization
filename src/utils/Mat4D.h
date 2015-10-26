#ifndef MAT4D_H
#define MAT4D_H


#include <cassert> // assert
#include <iostream> // debug

#include "debug.h"

#include "Pos.h"
#include "Dims.h"

template <typename F>
class Mat4D 
{
private:
public:
    F* data;
    unsigned int w, h, d, m;
    unsigned int size;
    
public:
    
    class iterator : public Pos4
    {
    public:
        Mat4D<F>& mat;
        iterator(unsigned int x, unsigned int y, unsigned int z, unsigned int n, Mat4D<F>& mat) : Pos4(x, y, z, n), mat(mat) { }
        friend class Mat4Df;
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
                    if (z == mat.d)
                    {
                        z = 0;
                        n++;
                    }
                }
            }
            return *this;
        }
        iterator operator++(int) { iterator ret = *this; ++(*this); return ret; }
        F& operator*() { return mat.data[mat.d*mat.h*mat.w* n + mat.h*mat.w* z + mat.w* y + x]; }
        F* operator->() { return &*(*this); }
        Pos4 getPos() { return Pos4(x,y,z,n); }
        bool operator==(iterator other) { return &mat == &other.mat && x == other.x && y == other.y && z == other.z && n == other.n; }
        bool operator!=(iterator other) { return !(*this == other); }
        
        
    };
    iterator end() {    return iterator(0,0,0,m, *this); }
    iterator begin() {  return iterator(0,0,0,0, *this); }

    
private:
    bool dont_delete_data = false;
public:
    ~Mat4D()
    {
        if (!dont_delete_data && data)
        {
            delete[] data;
        }
    }
protected:
    Mat4D<F>(unsigned int w, unsigned int h, unsigned int d, unsigned int m)
    : w(w), h(h), d(d), m(m), size(w*h*d*m)
    {
        data = new F[w*h*d*m];
    }
    
public:
    Mat4D<F>(Dims4 dims)
    : Mat4D<F>(dims.w, dims.h, dims.d, dims.m)
    {
    };
    
    Mat4D<F>& operator=(const Mat4D<F>& b)
    {
        w = b.w;
        h = b.h;
        d = b.d;
        m = b.m;
        size = b.size;
        std::cerr << "Warning! expensive Mat4Df assignment!\n";
        data = new float[w*h*d*m];
        for (unsigned int idx = 0; idx < size; idx++)
        {
            data[idx] = b.data[idx];
        }
        return *this;
    }
    
    /*!
     * Copy constructor
     */
    Mat4D<F>(const Mat4D<F>& b) 
    : w(b.w), h(b.h), d(b.d), m(b.m)
    , size(b.size)
    {
        std::cerr << "Warning! expensive Mat4Df copy!\n";
        data = new F[w*h*d*m];
        for (unsigned int idx = 0; idx < size; idx++)
        {
            data[idx] = b.data[idx];
        }
    }
    
    /*!
     * Move 'constructor'
     */
    Mat4D<F>(Mat4D<F>&& from)
    : w(w), h(h), d(d), m(m)
    , size(from.size)
    , data(from.data)
    {
        from.dont_delete_data;
    }
    Dims4 getDims()
    {
        return Dims4(w, h, d, m);
    }
    
    void clear(F& val)
    {
        for (int i = 0; i < size; i++)
            data[i] = val;
    }
    F& get(unsigned int x, unsigned int y, unsigned int z, unsigned int n)
    {
        assert(x<w && y<h && z<d && n < m);
        return data[d*w*h* n + w*h* z + w* y + x];
    }
    F& get(Pos4 p) { return get(p.x, p.y, p.z, p.n); }
    void set(unsigned int x, unsigned int y, unsigned int z, unsigned int n, F& val)
    {
        data[w*h*d* n + w*h* z + w* y + x] = val;
    }
    void set(Pos4 p, F& val) { return set(p.x, p.y, p.z, p.n, val); };
public:
    
    template <class Function>
    void apply(Function f, Mat4D<F>& result)
    {
        for (unsigned int rp = 0; rp < result.size; rp++)
        {
            result.data[rp] = f(data[rp]);
        }
    }
    template <class Function>
    Mat4D<F> apply(Function f)
    {
        Mat4D<F> result(w, h, d, m);
        apply(f, result);
        return result;
    }
};




#endif // MAT4D_H