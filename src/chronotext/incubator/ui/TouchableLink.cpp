/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/incubator/ui/TouchableLink.h"

using namespace ci;

namespace chronotext
{
    TouchableLink::TouchableLink(Shape *shape, int linkIndex, const ci::Rectf &bounds, int type, int hitExtra)
    :
    Touchable(shape, type, hitExtra),
    linkIndex(linkIndex),
    bounds(bounds)
    {}
    
    Rectf TouchableLink::getTouchableBounds()
    {
        return bounds;
    }
}
