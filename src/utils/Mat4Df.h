#ifndef MAT4DF_H
#define MAT4DF_H

#include <cassert> // assert

#include <iostream> // debug

#include "debug.h"
#include "Mat4D.h"


#include "Pos.h"
#include "Dims.h"


class Mat4Df : public Mat4D<float>
{
protected:
    Mat4Df(unsigned int w, unsigned int h, unsigned int d, unsigned int m)
    : Mat4D<float>(w, h, d, m)
    {
    }
    
public:
    Mat4Df(Dims4 dims)
    : Mat4D<float>(dims)
    {
    };
    
    Mat4Df& operator=(const Mat4Df& b)
    {
        Mat4D<float>::operator=(b);
        return *this;
    }
    
    /*!
     * Copy constructor
     */
    Mat4Df(const Mat4Df& b) 
    : Mat4D<float>(b)
    {
    }
    
    /*!
     * Move 'constructor'
     */
    Mat4Df(Mat4Df&& from)
    : Mat4D<float>(from)
    {
    }
    
    
    void clear(float val = 0.0) { Mat4D<float>::clear(val); }
    
    void set(Pos4 p, float val) { Mat4D<float>::set(p, val); };
    void set(unsigned int x, unsigned int y, unsigned int z, unsigned int n, float val) { Mat4D<float>::set(x,y,z,n, val); }
    
    void add(Pos4 p, float val) { data[w*h*d* p.n + w*h* p.z + w* p.y + p.x] += val; };
    void add(unsigned int x, unsigned int y, unsigned int z, unsigned int n, float val) { data[w*h*d* n + w*h* z + w* y + x] += val; };
    
    template <class Function>
    void apply(Function f, Mat4Df& result)
    {
        for (unsigned int rp = 0; rp < result.size; rp++)
        {
            result.data[rp] = f(data[rp]);
        }
    }
    
    template <class Function>
    Mat4Df apply(Function f)
    {
        Mat4Df result(w, h, d, m);
        apply(f, result);
        return result;
    }
    
    template <class Function>
    void applyInPlace(Function f)
    {
        for (unsigned int rp = 0; rp < size; rp++)
        {
            data[rp] = f(data[rp]);
        }
    }
    
    void debugOut(std::string name = "")
    {
        std::cerr << name << ": " << std::endl;
        for (Mat4Df::iterator in_it = begin(); in_it != end(); ++in_it)
        {
            if (in_it != begin() && in_it.x == 0) 
            {
                std::cerr << std::endl;
                if (in_it.y == 0) 
                {
                    if (in_it.z == 0)
                        std::cerr << "==============";
                    std::cerr << std::endl;
                }
            }
            if (*in_it >= 0) // same spacing between begative and positive numbers
                std::cerr << " ";
            std::cerr << *in_it << "\t";
        }
        std::cerr << "\n";
    }
};


#endif//MAT4DF_H
