/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/incubator/ui/Touchable.h"
#include "chronotext/incubator/ui/Shape.h"

using namespace ci;

namespace chronotext
{
    Rectf Touchable::getTouchableBounds()
    {
        return Rectf(0, 0, 0, 0);
    }
    
    bool Touchable::hitTest(const Vec2f &point, float *distance)
    {
        Rectf extra = getTouchableBounds().inflated(Vec2f(hitExtra, hitExtra));
        
        if (extra.contains(point))
        {
            *distance = getTouchableBounds().distance(point);
            return true;
        }
        
        return false;
    }
    
    void Touchable::changeState(int nextState)
    {
        if (state != nextState)
        {
            shape->touchStateChanged(this, nextState, state);
            state = nextState;
        }
    }
    
    void Touchable::performAction(int action)
    {
        shape->touchActionPerformed(this, action);
    }
}
