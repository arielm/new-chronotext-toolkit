#pragma once

#include "chronotext/incubator/ui/ShapeStyle.h"

namespace chronotext
{
    typedef boost::shared_ptr<class LayoutStyle> LayoutStyleRef;

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
