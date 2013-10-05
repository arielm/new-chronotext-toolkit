/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/incubator/ui/Touchable.h"
#include "chronotext/incubator/ui/Shape.h"

namespace chronotext
{
    void Touchable::changeState(int state)
    {
        this->state = state;
        shape->touchStateChanged(this, state);
    }
    
    void Touchable::performAction(int action)
    {
        shape->touchActionPerformed(this, action);
    }
}
