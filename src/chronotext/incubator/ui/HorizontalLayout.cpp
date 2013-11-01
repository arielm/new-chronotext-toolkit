/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/incubator/ui/HorizontalLayout.h"

using namespace std;
using namespace ci;

namespace chronotext
{
    HorizontalLayout::HorizontalLayout()
    :
    Container()
    {}
    
    HorizontalLayout::HorizontalLayout(LayoutStyleRef style)
    :
    Container(style),
    alignX(style->alignX),
    alignY(style->alignY),
    fill(style->fill)
    {}
    
    void HorizontalLayout::setAlign(int x, int y)
    {
        alignX = x;
        alignY = y;
        layoutRequest = true;
    }
    
    void HorizontalLayout::layout()
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
                    contentWidth += mergedMargin(previousMargin, component->marginLeft);
                    contentWidth += component->getWidth();
                    
                    contentHeight = max<float>(contentHeight, component->marginTop + component->getHeight() + component->marginBottom);
                    previousMargin = component->marginRight;
                }
            }
            
            contentWidth += previousMargin;
            
            if (autoWidth)
            {
                width = paddingLeft + contentWidth + paddingRight;
            }
            
            if (autoHeight)
            {
                height = paddingTop + contentHeight + paddingBottom;
            }
            
            // ---

            float innerWidth = width - paddingLeft - paddingRight;
            float left = paddingLeft;
            
            switch (alignX)
            {
                case ALIGN_MIDDLE:
                    left += (innerWidth - contentWidth) * 0.5f;
                    break;
                    
                case ALIGN_RIGHT:
                    left += innerWidth - contentWidth;
                    break;
            }
            
            // ---
            
            float innerHeight = height - paddingTop - paddingBottom;
            previousMargin = 0;

            for (auto component : components)
            {
                if (component->visible)
                {
                    float top = paddingTop;
                    
                    switch (alignY)
                    {
                        case ALIGN_TOP:
                            top += component->marginTop;
                            break;
                            
                        case ALIGN_MIDDLE:
                            top += (innerHeight - component->getHeight()) * 0.5f; // XXX
                            break;
                            
                        case ALIGN_BOTTOM:
                            top += innerHeight - component->getHeight() - component->marginBottom;
                            break;
                    }

                    left += mergedMargin(previousMargin, component->marginLeft);
                    component->setLocation(x + left, y + top);
                    
                    left += component->getWidth();
                    previousMargin = component->marginRight;
                }
            }
        }
        
        layoutRequest = false;
    }
    
    void HorizontalLayout::draw()
    {
        layout();
        fill.draw(getBounds());
        drawComponents();
    }
}
