/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/incubator/ui/Container.h"

namespace chronotext
{
    class TouchGroup
    {
    public:
        TouchGroup();
        
        void addShape(Shape *shape);
        void addShapes(const std::vector<ShapeRef> &shapes);
        
        void toggleTouchable(Touchable *touchable);
        void clickTouchable(Touchable *touchable);
        
        bool addTouch(int index, const ci::Vec2f &point);
        bool updateTouch(int index, const ci::Vec2f &point);
        bool removeTouch(int index, const ci::Vec2f &point);
        
    protected:
        Touchable* toggledTouchable;
        bool toggledOrDisabledIsArmed;
        
        std::vector<Shape*> shapes;
        
        Touchable* getClosestTouchable(const ci::Vec2f &point);
        Touchable* getArmedTouchableByIndex(int index);
    };
}

namespace chr = chronotext;
