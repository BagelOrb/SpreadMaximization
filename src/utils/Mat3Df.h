#ifndef MAT3DF_H
#define MAT3DF_H

#include <cassert> // assert

#include <iostream> // debug

#include "debug.h"
#include "Mat3D.h"


#include "Pos.h"
#include "Dims.h"


class Mat3Df : public Mat3D<float>
{
protected:
    Mat3Df(unsigned int w, unsigned int h, unsigned int d)
    : Mat3D<float>(w, h, d)
    {
    }
    
public:
    Mat3Df(Dims dims)
    : Mat3D<float>(dims)
    {
    };
    
    Mat3Df& operator=(const Mat3Df& b)
    {
        Mat3D<float>::operator=(b);
        return *this;
    }
    
    /*!
     * Copy constructor
     */
    Mat3Df(const Mat3Df& b) 
    : Mat3D<float>(b)
    {
    }
    
    /*!
     * Move 'constructor'
     */
    Mat3Df(Mat3Df&& from)
    : Mat3D<float>(from)
    {
    }
    
    
    void clear(float val = 0.0) { Mat3D<float>::clear(val); }
    
    void set(Pos p, float val) { Mat3D<float>::set(p, val); };
    void set(unsigned int x, unsigned int y, unsigned int z, float val) { Mat3D<float>::set(x,y,z, val); }
    
    void add(Pos p, float val) { data[w*h* p.z + w* p.y + p.x] += val; };
    void add(unsigned int x, unsigned int y, unsigned int z, float val) { data[w*h* z + w* y + x] += val; };
    
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
    
    void debugOut(std::string name = "")
    {
        std::cerr << name << ": " << std::endl;
        for (Mat3Df::iterator in_it = begin(); in_it != end(); ++in_it)
        {
            if (in_it != begin() && in_it.x == 0) 
            {
                std::cerr << std::endl;
                if (in_it.y == 0) 
                    std::cerr << std::endl;
            }
            if (*in_it >= 0)
                std::cerr << " ";
            std::cerr << *in_it << "\t";
        }
        std::cerr << "\n";
    }
};


#endif//MAT3DF_H
