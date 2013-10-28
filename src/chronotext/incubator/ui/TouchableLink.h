/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/incubator/ui/Touchable.h"

#include "cinder/Rect.h"

namespace chronotext
{
    class TouchableLink : public Touchable
    {
    public:
        ci::Rectf bounds;
        int linkIndex;
        
        TouchableLink(Shape *shape, int linkIndex, const ci::Rectf &bounds, int type, int hitExtra);
        
        ci::Rectf getTouchableBounds();
    };
}

namespace chr = chronotext;
