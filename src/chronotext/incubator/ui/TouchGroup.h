#pragma once

#include "chronotext/incubator/ui/Shape.h"
#include "chronotext/incubator/ui/Container.h"

#include <vector>

namespace chronotext
{
    class TouchGroup
    {
        Touchable* toggledTouchable;
        bool toggledOrDisabledIsArmed;
        
        std::vector<Shape*> shapes;
        
        Touchable* getClosestTouchable(const ci::Vec2f &point);
        Touchable* getArmedTouchableByIndex(int index);

    public:
        TouchGroup()
        :
        toggledTouchable(NULL),
        toggledOrDisabledIsArmed(false)
        {}
        
        void addShape(Shape *shape);
        void addShapes(const std::vector<ShapeRef> &shapes);
        
        void toggleTouchable(Touchable *touchable);
        void clickTouchable(Touchable *touchable);
        
        bool addTouch(int index, float x, float y);
        bool updateTouch(int index, float x, float y);
        bool removeTouch(int index, float x, float y);
    };
}

namespace chr = chronotext;
