/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/incubator/ui/TouchGroup.h"

#include <limits>

using namespace std;
using namespace ci;

namespace chr
{
    TouchGroup::TouchGroup()
    :
    toggledTouchable(NULL),
    toggledOrDisabledIsArmed(false)
    {}
    
    void TouchGroup::addShape(Shape *shape)
    {
        shapes.push_back(shape);
    }
    
    void TouchGroup::addShapes(const vector<ShapeRef> &shapes)
    {
        for (auto &shape : shapes)
        {
            this->shapes.push_back(shape.get());
        }
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
    
    bool TouchGroup::addTouch(int index, const Vec2f &point)
    {
        Touchable* closestTouchable = getClosestTouchable(point);
        
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
    
    bool TouchGroup::updateTouch(int index, const Vec2f &point)
    {
        Touchable *armedTouchable = getArmedTouchableByIndex(index);
        
        if (armedTouchable)
        {
            if (armedTouchable == getClosestTouchable(point))
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
    
    bool TouchGroup::removeTouch(int index, const Vec2f &point)
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
    
    Touchable* TouchGroup::getClosestTouchable(const Vec2f &point)
    {
        Touchable *closestTouchable = NULL;
        float closestDistance = numeric_limits<float>::max();
        
        for (auto &shape : shapes)
        {
            if (shape->visible)
            {
                for (auto &touchable : shape->getTouchables())
                {
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
        for (auto &shape : shapes)
        {
            for (auto &touchable : shape->getTouchables())
            {
                if (touchable->armedIndex == index)
                {
                    return touchable;
                }
            }
        }
        
        return NULL;
    }
}
