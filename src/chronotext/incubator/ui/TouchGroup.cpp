#include "chronotext/incubator/ui/TouchGroup.h"

#include <limits>

using namespace std;
using namespace ci;

namespace chronotext
{
    void TouchGroup::addShape(Shape *shape)
    {
        shapes.push_back(shape);
    }
    
    void TouchGroup::addShapes(const vector<ShapeRef> &shapes)
    {
        for (vector<ShapeRef>::const_iterator it = shapes.begin(); it != shapes.end(); ++it)
        {
            this->shapes.push_back(&**it);
        }
    }
    
    Touchable* TouchGroup::getClosestTouchable(const Vec2f &point)
    {
        Touchable *closestTouchable = NULL;
        float closestDistance = numeric_limits<float>::max();
        
        for (vector<Shape*>::const_iterator it1 = shapes.begin(); it1 != shapes.end(); ++it1)
        {
            Shape *shape = *it1;
            
            if (shape->visible)
            {
                vector<Touchable*> touchables = shape->getTouchables();
                
                for (vector<Touchable*>::const_iterator it2 = touchables.begin(); it2 != touchables.end(); ++it2)
                {
                    Touchable *touchable = *it2;
                    
                    float distance;
                    if (touchable->hitTest(point, &distance))
                    {
                        if (distance < closestDistance)
                        {
                            closestDistance = distance;
                            closestTouchable = touchable;
                        }
                    }
                }
            }
        }
        
        return closestTouchable;
    }
    
    Touchable* TouchGroup::getArmedTouchableByIndex(int index)
    {
        for (vector<Shape*>::const_iterator it1 = shapes.begin(); it1 != shapes.end(); ++it1)
        {
            Shape *shape = *it1;
            vector<Touchable*> touchables = shape->getTouchables();
            
            for (vector<Touchable*>::const_iterator it2 = touchables.begin(); it2 != touchables.end(); ++it2)
            {
                Touchable *touchable = *it2;
                
                if (touchable->armedIndex == index)
                {
                    return touchable;
                }
            }
        }
        
        return NULL;
    }
    
    void TouchGroup::toggleTouchable(Touchable *touchable)
    {
        if (toggledTouchable)
        {
            toggledTouchable->changeState(Touchable::STATE_NORMAL);
        }
        
        toggledTouchable = touchable;

        touchable->changeState(Touchable::STATE_TOGGLED);
        touchable->performAction(Touchable::ACTION_TOGGLED);
    }
    
    void TouchGroup::clickTouchable(Touchable *touchable)
    {
        touchable->changeState(Touchable::STATE_NORMAL);
        touchable->performAction(Touchable::ACTION_CLICKED);
    }

    bool TouchGroup::addTouch(int index, float x, float y)
    {
        Touchable* closestTouchable = getClosestTouchable(Vec2f(x, y));
        
        if (closestTouchable)
        {
            if (closestTouchable->state == Touchable::STATE_DISABLED)
            {
                toggledOrDisabledIsArmed = true;
                return true;
            }
            else if (closestTouchable->type == Touchable::TYPE_TOGGLABLE)
            {
                if (closestTouchable->state == Touchable::STATE_NORMAL)
                {
                    toggleTouchable(closestTouchable);
                }
                
                toggledOrDisabledIsArmed = true;
                return true;
            }
            else if (closestTouchable->type == Touchable::TYPE_CLICKABLE)
            {
                if (closestTouchable->armedIndex == -1)
                {
                    closestTouchable->armedIndex = index;
                    closestTouchable->changeState(Touchable::STATE_PRESSED);
                    return true;
                }
            }
        }
        
        return false;
    }
    
    bool TouchGroup::updateTouch(int index, float x, float y)
    {
        Touchable *armedTouchable = getArmedTouchableByIndex(index);
        
        if (armedTouchable)
        {
            if (armedTouchable == getClosestTouchable(Vec2f(x, y)))
            {
                armedTouchable->changeState(Touchable::STATE_PRESSED);
                return true;
            }
            else
            {
                armedTouchable->changeState(Touchable::STATE_NORMAL);
                return true;
            }
        }
        
        return toggledOrDisabledIsArmed;
    }

    bool TouchGroup::removeTouch(int index, float x, float y)
    {
        Touchable *armedTouchable = getArmedTouchableByIndex(index);
        
        if (armedTouchable)
        {
            armedTouchable->armedIndex = -1;
            
            if (armedTouchable->state == Touchable::STATE_PRESSED)
            {
                clickTouchable(armedTouchable);
                return true;
            }
        }
        
        if (toggledOrDisabledIsArmed)
        {
            toggledOrDisabledIsArmed = false;
            return true;
        }
        
        return false;
    }
}
