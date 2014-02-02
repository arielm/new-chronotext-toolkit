/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/gl/gl.h"

#include <vector>

namespace chronotext
{
    struct TexturedTriangleStrip
    {
        std::vector<ci::Vec2f> vertices;
        std::vector<ci::Vec2f> coords;
        
        void clear()
        {
            vertices.clear();
            coords.clear();
        }
        
        bool empty() const
        {
            return vertices.empty();
        }
        
        void draw() const
        {
            glVertexPointer(2, GL_FLOAT, 0, vertices.data());
            glTexCoordPointer(2, GL_FLOAT, 0, coords.data());
            glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());
        }
    };
}
