#ifndef MAT3D_H
#define MAT3D_H


#include <cassert> // assert
#include <iostream> // debug

#include "debug.h"

#include "Pos.h"
#include "Dims.h"

template <typename F>
class Mat3D
{
private:
public:
    F* data;
    unsigned int w, h, d;
    unsigned int size;
    
public:
    
    class iterator : public Pos3
    {
    public:
//         unsigned int x, y, z;
        Mat3D<F>& mat;
        iterator(unsigned int x, unsigned int y, unsigned int z, Mat3D<F>& mat) : Pos3(x, y, z), mat(mat) { }
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
        F& operator*() { return mat.data[mat.w*mat.h* z + mat.w* y + x]; }
        F* operator->() { return &*(*this); }
        Pos3 getPos() { return Pos3(x,y,z); }
        bool operator==(iterator other) { return &mat == &other.mat && x == other.x && y == other.y && z == other.z; }
        bool operator!=(iterator other) { return !(*this == other); }
        
        
    };
    iterator end() { return iterator(0,0,d, *this); }
    iterator begin() { return iterator(0,0,0, *this); }

    
private:
    bool dont_delete_data = false;
public:
    ~Mat3D()
    {
        if (!dont_delete_data && data)
        {
            delete[] data;
        }
    }
protected:
    Mat3D<F>(unsigned int w, unsigned int h, unsigned int d)
    : w(w), h(h), d(d), size(w*h*d)
    {
        data = new F[w*h*d];
    }
    
public:
    Mat3D<F>(Dims3 dims)
    : Mat3D<F>(dims.w, dims.h, dims.d)
    {
    };
    
    Mat3D<F>& operator=(const Mat3D<F>& b)
    {
        w = b.w;
        h = b.h;
        d = b.d;
        size = b.size;
//         std::cerr << "Warning! expensive Mat3Df assignment!\n";
        data = new float[w*h*d];
        for (unsigned int idx = 0; idx < size; idx++)
        {
            data[idx] = b.data[idx];
        }
        return *this;
    }
    
    /*!
     * Copy constructor
     */
    Mat3D<F>(const Mat3D<F>& b) 
    : w(b.w)
    , h(b.h)
    , d(b.d)
    , size(b.size)
    {
//         std::cerr << "Warning! expensive Mat3Df copy!\n";
        data = new F[w*h*d];
        for (unsigned int idx = 0; idx < size; idx++)
        {
            data[idx] = b.data[idx];
        }
    }
    
    /*!
     * Move 'constructor'
     */
    Mat3D<F>(Mat3D<F>&& from)
    : w(from.w)
    , h(from.h)
    , d(from.d)
    , size(from.size)
    , data(from.data)
    {
        from.dont_delete_data;
    }
    Dims3 getDims()
    {
        return Dims3(w, h, d);
    }
    
    void clear(F& val)
    {
        for (int i = 0; i < w*h*d; i++)
            data[i] = val;
    }
protected:
    F& get(unsigned int x, unsigned int y, unsigned int z)
    {
        assert(x<w && y<h && z<d);
        return data[w*h* z + w* y + x];
    }
    void set(unsigned int x, unsigned int y, unsigned int z, F& val)
    {
        data[w*h* z + w* y + x] = val;
    }
public:
    F& get(Pos3 p) { return get(p.x, p.y, p.z); }
    void set(Pos3 p, F& val) { return set(p.x, p.y, p.z, val); };
public:
    
    template <class Function>
    void apply(Function f, Mat3D<F>& result)
    {
        for (unsigned int rp = 0; rp < result.size; rp++)
        {
            result.data[rp] = f(data[rp]);
        }
    }
    template <class Function>
    Mat3D<F> apply(Function f)
    {
        Mat3D<F> result(w, h, d);
        apply(f, result);
        return result;
    }
};




#endif // MAT3D_H