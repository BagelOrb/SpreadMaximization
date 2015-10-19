#ifndef MAT3DF_H
#define MAT3DF_H

#include <cassert> // assert

#include <iostream> // debug

#include "debug.h"



#include "Pos.h"


class Mat3Df
{
private:
public:
    float* data;
    unsigned int w, h, d;
    unsigned int size;
    
public:
    
    class iterator : public Pos
    {
    public:
//         unsigned int x, y, z;
        Mat3Df& mat;
        iterator(unsigned int x, unsigned int y, unsigned int z, Mat3Df& mat) : Pos(x, y, z), mat(mat) { }
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
//         Pos operator+(iterator other) { return Pos(x+other.x, y+other.y, z+other.z); }
        Pos getPos() { return Pos(x,y,z); }
        bool operator==(iterator other) { return &mat == &other.mat && x == other.x && y == other.y && z == other.z; }
        bool operator!=(iterator other) { return !(*this == other); }
        
        
    };
    iterator end() { return iterator(0,0,d, *this); }
    iterator begin() { return iterator(0,0,0, *this); }

private:
    Mat3Df(unsigned int w, unsigned int h, unsigned int d)
    : w(w), h(h), d(d), size(w*h*d)
    {
        data = new float[w*h*d];
    }
    
public:
    Mat3Df(Dims dims)
    : Mat3Df(dims.w, dims.h, dims.d)
    {
    };
    
private:
    bool dont_delete_data = false;
public:
    ~Mat3Df()
    {
        if (!dont_delete_data && data)
        {
            delete[] data;
        }
    }
    
    Mat3Df& operator=(const Mat3Df& b)
    {
        w = b.w;
        h = b.h;
        d = b.d;
        size = b.size;
        std::cerr << "Warning! expensive Mat3Df assignment!\n";
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
    Mat3Df(const Mat3Df& b) 
    : w(b.w)
    , h(b.h)
    , d(b.d)
    , size(b.size)
    {
        std::cerr << "Warning! expensive Mat3Df copy!\n";
        data = new float[w*h*d];
        for (unsigned int idx = 0; idx < size; idx++)
        {
            data[idx] = b.data[idx];
        }
    }
    
    /*!
     * Move 'constructor'
     */
    Mat3Df(Mat3Df&& from)
    : w(from.w)
    , h(from.h)
    , d(from.d)
    , size(from.size)
    , data(from.data)
    {
        from.dont_delete_data;
    }
    
    Dims getDims()
    {
        return Dims(w, h, d);
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
                result.add(res_it, get(dataPos) * *k_it);
            }
        }
    }
public:
    /*
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
    }*/
    
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
    void debugOut()
    {
        for (Mat3Df::iterator in_it = begin(); in_it != end(); ++in_it)
        {
            if (in_it.x == 0) 
            {
                std::cerr << std::endl;
                if (in_it.y == 0) 
                    std::cerr << std::endl;
            }
            std::cerr << *in_it << "\t";
        }
        std::cerr << "\n";
    }
};


#endif//MAT3DF_H
