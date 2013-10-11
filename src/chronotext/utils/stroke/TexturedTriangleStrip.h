/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/gl/gl.h"

namespace chronotext
{
    struct TexturedTriangleStrip
    {
        int count;
        float *vertices;
        float *coords;
        
        TexturedTriangleStrip()
        :
        count(0),
        vertices(NULL),
        coords(NULL)
        {}
        
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
                
                count = 0;
                vertices = NULL;
                coords = NULL;
            }
        }
        
        void draw() const
        {
            glTexCoordPointer(2, GL_FLOAT, 0, coords);
            glVertexPointer(2, GL_FLOAT, 0, vertices);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, count);
        }
    };
}
