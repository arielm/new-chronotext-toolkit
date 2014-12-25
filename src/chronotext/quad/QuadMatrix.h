/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/quad/QuadBatch.h"

#include "cinder/Matrix44.h"

#include <array>

namespace chr
{
    class QuadMatrix
    {
    public:
        typedef std::array<float, 16> Values;
        
        union
        {
            Values m;
            
            struct
            {
                float m00, m10, m20, m30;
                float m01, m11, m21, m31;
                float m02, m12, m22, m32;
                float m03, m13, m23, m33;
            };
        };
        
        QuadMatrix();

        void load(const ci::Matrix44f &matrix);
        void load(const ci::MatrixAffine2f &matrix);

        void setToIdentity();
        
        void push();
        void pop();

        inline void setTranslation(const ci::Vec2f &t) { setTranslation(t.x, t.y, 0); }
        inline void setTranslation(const ci::Vec3f &t) { setTranslation(t.x, t.y, t.z); }
        void setTranslation(float x, float y, float z = 0);

        inline void translate(const ci::Vec2f &t) { translate(t.x, t.y, 0); }
        inline void translate(const ci::Vec3f &t) { translate(t.x, t.y, t.z); }
        void translate(float x, float y, float z = 0);
        
        inline void scale(float s) { scale(s, s, s); }
        void scale(float x, float y, float z = 1);
        
        void rotateX(float a);
        void rotateY(float a);
        void rotateZ(float a);
        void rotateXY(float sx, float sy);
        
        ci::Vec3f transform(float x, float y) const;
        void addTransformedQuad(const Quad &quad, std::vector<Vertex> &vertices) const;
        
    protected:
        std::vector<Values> stack;
    };
}
