/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * BASED ON PMatrix2D.java FROM PROCESSING: http://code.google.com/p/processing
 */

#pragma once

#include "cinder/Vector.h"

namespace chronotext
{
    class Matrix2D
    {
        float m00, m01, m02;
        float m10, m11, m12;
        
    public:
        Matrix2D() : m00(1), m01(0), m02(0), m10(0), m11(1), m12(0) {}
        
        void setToIdentity();
        void translate(float x, float y);
        void scale(float s);
        void rotate(float a);
        ci::Vec2f transform(float x, float y) const;
    };
}

namespace chr = chronotext;
