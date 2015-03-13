/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * BASED ON:
 * https://github.com/cinder/Cinder/blob/master/include/cinder/Triangulate.h
 * https://github.com/openframeworks/openFrameworks/blob/master/libs/openFrameworks/graphics/ofTessellator.h
 *
 * WHY NOT USING CINDER'S Triangulator?
 * - WE NEED MORE INPUT FORMATS (E.G. vector<vector<Vec2f>>)
 * - WE NEED 16-BIT VERTEX-INDICES FOR COMPATIBILITY WITH OPENGL-ES
 */

#pragma once

#include "chronotext/path/ShapeMesh.h"
#include "chronotext/path/FXGDocument.h"

#include "cinder/Shape2d.h"

#include "tesselator.h"

namespace chr
{
    class ShapeTesselator
    {
    public:
        ShapeTesselator();
        ~ShapeTesselator();

        void add(const ci::Rectf &rect);
        void add(const ci::Shape2d &shape, float approximationScale = 1.0f);
        void add(const ci::Path2d &path, float approximationScale = 1.0f);
        void add(const FXGDocument &document, float approximationScale = 1.0f);
        void add(const std::vector<std::vector<ci::Vec2f>> &polygons);
        void add(const std::vector<ci::Vec2f> &polygon);
        
        ShapeMesh* createMesh(int windingRule = TESS_WINDING_ODD);
        
    protected:
        TESStesselator *tess;
    };
}
