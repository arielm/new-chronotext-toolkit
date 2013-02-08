#pragma once

#include "cinder/gl/gl.h"

struct TexturedTriangleStrip
{
    int count;
    float *vertices;
    float *coords;
    
    TexturedTriangleStrip() : count(0), vertices(NULL), coords(NULL) {}
    
    ~TexturedTriangleStrip()
    {
        clear();
    }
    
    void clear()
    {
        if (vertices)
        {
            delete[] vertices;
            delete[] coords;
        }
    }
    
    void draw()
    {
        glTexCoordPointer(2, GL_FLOAT, 0, coords);
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, count);
    }
};
