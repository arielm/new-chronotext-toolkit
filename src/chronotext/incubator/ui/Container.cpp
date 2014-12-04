/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/incubator/ui/Container.h"

using namespace std;
using namespace ci;

namespace chr
{
    Container::Container()
    :
    Shape()
    {}

    Container::Container(shared_ptr<Shape::Style> style)
    :
    Shape(style),
    layoutRequest(false)
    {}
    
    void Container::setWidth(float newWidth)
    {
        if (autoWidth || newWidth != width)
        {
            layoutRequest = true;
            Shape::setWidth(newWidth);
        }
    }
    
    void Container::setHeight(float newHeight)
    {
        if (autoHeight || newHeight != height)
        {
            layoutRequest = true;
            Shape::setHeight(newHeight);
        }
    }
    
    void Container::setAutoWidth(bool autoWidth)
    {
        if (autoWidth != this->autoWidth)
        {
            layoutRequest = true;
            Shape::setAutoWidth(autoWidth);
        }
    }
    
    void Container::setAutoHeight(bool autoHeight)
    {
        if (autoHeight != this->autoHeight)
        {
            layoutRequest = true;
            Shape::setAutoHeight(autoHeight);
        }
    }
    
    void Container::setPadding(float left, float top, float right, float bottom)
    {
        layoutRequest = true;
        Shape::setPadding(left, top, right, bottom);
    }
    
    float Container::getWidth()
    {
        layout();
        return width;
    }
    
    float Container::getHeight()
    {
        layout();
        return height;
    }
    
    Shape* Container::getComponentByTag(int tag)
    {
        for (auto &component : components)
        {
            if (component->tag == tag)
            {
                return component.get();
            }
        }
        
        return NULL;
    }
    
    void Container::addComponent(ShapeRef component)
    {
        layoutRequest = true;
        components.push_back(component);
        component->setContainer(this);
    }
    
    void Container::requestLayout()
    {
        layoutRequest = true;
    }
    
    void Container::draw()
    {
        layout();
        drawComponents();
    }
    
    void Container::drawComponents()
    {
        for (auto &component : components)
        {
            if (component->visible)
            {
                component->draw();
            }
        }
    }
    
    float Container::mergedMargin(float previousMargin, float nextMargin)
    {
        if ((previousMargin > 0) && (nextMargin > 0))
        {
            return max<float>(previousMargin, nextMargin);
        }
        else if ((previousMargin < 0) && (nextMargin < 0))
        {
            return min<float>(previousMargin, nextMargin);
        }
        
        return previousMargin + nextMargin;
    }
}
