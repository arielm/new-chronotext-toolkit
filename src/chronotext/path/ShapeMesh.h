/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * WHY NOT USING CINDER'S TriMesh2d?
 * -  WE NEED 16-BIT VERTEX-INDICES FOR COMPATIBILITY WITH OPENGL-ES
 */

#pragma once

#include "cinder/Vector.h"

#include <vector>

namespace chr
{
    class ShapeMesh
    {
    public:
        void appendVertices(const ci::Vec2f *vertices, size_t count);
        void appendIndices(const uint32_t *indices, size_t count);
        
        const std::vector<ci::Vec2f>& getVertices() const;
        const std::vector<uint16_t>& getIndices() const;
        
        void clear();
        void draw();
        
    protected:
        std::vector<ci::Vec2f> vertices;
        std::vector<uint16_t> indices;
    };
}
