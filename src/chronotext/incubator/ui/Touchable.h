/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Rect.h"

namespace chr
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
        
        Touchable(Shape *shape, int type, int hitExtra = 0);
        virtual ~Touchable() {}

        virtual ci::Rectf getTouchableBounds(); // ONLY RELEVANT FOR RECTANGULAR-TOUCHABLES
        virtual bool hitTest(const ci::Vec2f &point, float *distance); // USE THE DEFAULT (I.E. RECTANGULAR) getTouchableBounds()
        
        void changeState(int nextState);
        void performAction(int action);
    };
}
