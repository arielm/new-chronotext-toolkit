/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/incubator/ui/VerticalLayout.h"

using namespace std;
using namespace ci;

namespace chronotext
{
    VerticalLayout::VerticalLayout()
    :
    Container()
    {}
    
    VerticalLayout::VerticalLayout(shared_ptr<Shape::Style> style)
    :
    Container(style),
    alignX(style->alignX),
    alignY(style->alignY),
    fill(style->fill)
    {}
    
    void VerticalLayout::layout()
    {
        if (layoutRequest)
        {
            float previousMargin = 0;
            contentWidth = 0;
            contentHeight = 0;
            
            for (auto component : components)
            {
                if (component->visible)
                {
                    if (!component->autoWidth)
                    {
                        if (autoWidth)
                        {
                            throw runtime_error("VerticalLayout WITH AUTO-WIDTH MUST CONTAIN COMPONENTS WITH FIXED-WIDTH");
                        }
                        else
                        {
                            float availableWidth = max<float>(0, width - paddingLeft - paddingRight - component->marginLeft - component->marginRight);
                            component->setWidth(availableWidth);
                        }
                    }
                    
                    contentHeight += mergedMargin(previousMargin, component->marginTop);
                    contentHeight += component->getHeight();
                    
                    contentWidth = max<float>(contentWidth, component->marginLeft + component->getWidth() + component->marginRight);
                    previousMargin = component->marginBottom;
                }
            }
            
            if (autoWidth)
            {
                width = paddingLeft + contentWidth + paddingRight;
            }
            
            if (autoHeight)
            {
                height = paddingTop + contentHeight + paddingBottom;
            }
            
            // ---
            
            float innerHeight = height - paddingTop - paddingBottom;
            float top = paddingTop;

            switch (alignY)
            {
                case ALIGN_MIDDLE:
                    top += (innerHeight - contentHeight) * 0.5f;
                    break;
                    
                case ALIGN_BOTTOM:
                    top += innerHeight - contentHeight;
                    break;
            }
            
            // ---
            
            float innerWidth = width - paddingLeft - paddingRight;
            previousMargin = 0;
            
            for (auto component : components)
            {
                if (component->visible)
                {
                    float left = paddingLeft;
                    
                    switch (alignX)
                    {
                        case ALIGN_LEFT:
                            left += component->marginLeft;
                            break;
                            
                        case ALIGN_MIDDLE:
                            left += (innerWidth - component->getWidth()) * 0.5f; // XXX
                            break;
                            
                        case ALIGN_RIGHT:
                            left += innerWidth - component->getWidth() - component->marginRight;
                            break;
                    }
                    
                    top += mergedMargin(previousMargin, component->marginTop);
                    component->setLocation(x + left, y + top);
                    
                    top += component->getHeight();
                    previousMargin = component->marginBottom;
                }
            }
        }
        
        layoutRequest = false;
    }
    
    void VerticalLayout::draw()
    {
        layout();
        fill.draw(getBounds());
        drawComponents();
    }
}
