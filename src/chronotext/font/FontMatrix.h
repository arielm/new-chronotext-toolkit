/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Matrix44.h"
#include "cinder/Rect.h"

namespace chronotext
{
    struct GlyphQuad
    {
        float x1;
        float y1;
        float x2;
        float y2;
        
        float u1;
        float v1;
        float u2;
        float v2;
        
        GlyphQuad()
        {}
        
        GlyphQuad(const ci::Vec2f &position, const ci::Vec2f &size, const ci::Rectf &coords)
        :
        x1(position.x),
        y1(position.y),
        u1(coords.x1),
        v1(coords.y1),
        u2(coords.x2),
        v2(coords.y2)
        {
            x2 = x1 + size.x;
            y2 = y1 + size.y;
        }
    };
    
    class FontMatrix
    {
        union
        {
            float m[16];
            
            struct
            {
                float m00, m10, m20, m30;
                float m01, m11, m21, m31;
                float m02, m12, m22, m32;
                float m03, m13, m23, m33;
            };
        };
        
    public:
        FontMatrix();

        void load(const ci::Matrix44f &matrix);
        void load(const ci::MatrixAffine2f &matrix);

        void setToIdentity();

        inline void setTranslation(const ci::Vec2f &t) { setTranslation(t.x, t.y, 0); }
        void setTranslation(float x, float y, float z);

        inline void translate(const ci::Vec2f &t) { translate(t.x, t.y, 0); }
        void translate(float x, float y, float z);
        
        inline void scale(float s) { scale(s, s, s); }
        void scale(float x, float y, float z);
        
        void rotateX(float a);
        void rotateY(float a);
        void rotateZ(float a);
        void rotateXY(float sx, float sy);
        
        ci::Vec3f transform3D(float x, float y) const;
        ci::Vec2f transform2D(float x, float y) const;
        
        int addTransformedQuad3D(const GlyphQuad &quad, float *vertices) const;
        int addTransformedQuad2D(const GlyphQuad &quad, float *vertices) const;
    };
}

namespace chr = chronotext;
