#ifndef UTILS_MNIST_H
#define UTILS_MNIST_H

#include <fstream>

#include <iomanip> // std cerr precision TODO remove line

#include "Dataset.h"

#include "NoCopy.h"
#include "Dims.h"
#include "Mat3Df.h"
#include "Mat4Df.h"

class Mnist : NoCopy, public Dataset
{
    static constexpr int32_t magic_label = 2049;
    static constexpr int32_t magic_data = 2051;

    static Mnist* dataset;
    
public:
    static Mnist& getDataset() 
    { 
        if (!dataset)
        {
            dataset = new Mnist();
        }
        return *dataset; 
    }
    
private:
//     Mat4Df* data;
//     Mat4D<char>* labels;

    static int32_t readInt32(std::fstream& fs)
    {
        int32_t num;
        fs.read((char*)&num, sizeof(int32_t));
        return __builtin_bswap32(num);
    }
    static unsigned char readInt8(std::fstream& fs)
    {
        unsigned char num;
        fs.read((char*)&num, sizeof(char));
        return num;
    }
    
    static Dims4 readHeader(std::fstream& fs)
    {
        int w, h, d, m;
        d = 1;
        int32_t magic = readInt32(fs);
        assert(magic == magic_data || magic == magic_label);
        m = readInt32(fs);
        if (magic == magic_data)
        {
            w = readInt32(fs);
            h = readInt32(fs);
        }
        else 
        {
            w = h = 1;
        }
        return Dims4(w, h, d, m);
    }

    void readData()
    {
        std::fstream fs;
        fs.open("data/MNIST/train-images.idx3-ubyte", std::fstream::in);

        Dims4 dims = readHeader(fs);
        
        data = new Mat4Df(dims);
        
        for (unsigned int pixel_idx = 0 ; pixel_idx < data->size ; pixel_idx++)
        {
            data->data[pixel_idx] = readInt8(fs) / 255.0;
        }
        fs.close();
        
    }

    void readLabels()
    {
        std::fstream fs;
        fs.open("data/MNIST/train-labels.idx1-ubyte", std::fstream::in);

        Dims4 dims = readHeader(fs);
        
        labels = new Mat4D<char>(dims);
        
        for (unsigned int pixel_idx = 0 ; pixel_idx < labels->size ; pixel_idx++)
        {
            labels->data[pixel_idx] = readInt8(fs);
        }
        fs.close();
        
    }
    
    Mnist()
    : Dataset()
    {
        std::cerr << "reading MNIST data..." << std::endl;
        readData();
        std::cerr << "finished reading MNIST data.\n";
        readLabels();
    }


public:




};

Mnist* Mnist::dataset = nullptr;

#endif // UTILS_MNIST_H