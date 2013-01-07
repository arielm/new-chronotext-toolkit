#pragma once

#include "cinder/Vector.h"

#include <vector>

class Path3D
{
public:
    struct Value
    {
        float x;
        float y;
        float z;
        float ax;
        float ay;
        float az;
        float pos;
        
        Value() {}
        
        Value(float x, float y, float z, float ax, float ay, float az, float pos)
        :
        x(x),
        y(y),
        z(z),
        ax(ax),
        ay(ay),
        az(az),
        pos(pos)
        {}
    };
    
    std::vector<float> x;
    std::vector<float> y;
    std::vector<float> z;
    std::vector<float> ax;
    std::vector<float> ay;
    std::vector<float> az;
    std::vector<float> len;
    
    int size;
    
    Path3D() : size(0) {}

    void clear();
    float getLength();
    void add(float xx, float yy, float zz, float aax, float aaz);
    
    Path3D::Value pos2Value(float pos) const;
};
