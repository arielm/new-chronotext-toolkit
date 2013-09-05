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
        
        bool addTouch(int index, const ci::Vec2f &point);
        bool updateTouch(int index, const ci::Vec2f &point);
        bool removeTouch(int index, const ci::Vec2f &point);
    };
}

namespace chr = chronotext;
