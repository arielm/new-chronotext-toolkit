/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Vector.h"

#include <vector>

namespace chronotext
{
    class Shape;

    class Touchable
    {
    public:
        enum
        {
            STATE_NORMAL,
            STATE_PRESSED,
            STATE_TOGGLED,
            STATE_DISABLED,
        };
        
        enum
        {
            TYPE_CLICKABLE,
            TYPE_TOGGLABLE,
        };
        
        enum
        {
            ACTION_CLICKED,
            ACTION_TOGGLED,
        };
        
        Shape *shape;
        int state;
        int type;
        int armedIndex;
        float hitExtra;
        
        Touchable(Shape *shape, int type, int hitExtra = 0)
        :
        shape(shape),
        state(STATE_NORMAL),
        type(type),
        hitExtra(hitExtra),
        armedIndex(-1)
        {}
        
        virtual ~Touchable() {}
        
        virtual bool hitTest(const ci::Vec2f &point, float *distance) = 0;
        
        void changeState(int nextState);
        void performAction(int action);
    };
}

namespace chr = chronotext;
