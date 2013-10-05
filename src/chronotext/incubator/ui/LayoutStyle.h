/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/incubator/ui/ShapeStyle.h"

namespace chronotext
{
    typedef std::shared_ptr<class LayoutStyle> LayoutStyleRef;

    class LayoutStyle : public ShapeStyle
    {
    public:
        int alignX;
        int alignY;

        LayoutStyle()
        :
        ShapeStyle(),
        alignX(Shape::ALIGN_MIDDLE),
        alignY(Shape::ALIGN_MIDDLE)
        {}
    };
}

namespace chr = chronotext;
