#pragma once

#include "chronotext/incubator/ui/Touchable.h"

#include "cinder/Rect.h"

#include <string>

namespace chronotext
{
    class TouchableLink : public Touchable
    {
    public:
        ci::Rectf bounds;
        int linkIndex;
        
        TouchableLink(Shape *shape, int linkIndex, const ci::Rectf &bounds, int type, int hitExtra)
        :
        Touchable(shape, type, hitExtra),
        linkIndex(linkIndex),
        bounds(bounds)
        {}
        
        bool hitTest(const ci::Vec2f &point, float *distance);
    };
}

namespace chr = chronotext;
