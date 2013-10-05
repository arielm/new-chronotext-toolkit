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
    void VerticalLayout::layout()
    {
        if (layoutRequest)
        {
            float previousMargin = 0;
            contentWidth = 0;
            contentHeight = 0;
            
            for (vector<ShapeRef>::const_iterator it = components.begin(); it != components.end(); ++it)
            {
                ShapeRef shape = *it;
                
                if (shape->visible)
                {
                    if (!shape->autoWidth)
                    {
                        if (autoWidth)
                        {
                            throw runtime_error("VerticalLayout WITH AUTO-WIDTH MUST CONTAIN COMPONENTS WITH FIXED-WIDTH");
                        }
                        else
                        {
                            float availableWidth = max<float>(0, width - paddingLeft - paddingRight - shape->marginLeft - shape->marginRight);
                            shape->setWidth(availableWidth);
                        }
                    }
                    
                    contentHeight += mergedMargin(previousMargin, shape->marginTop);
                    contentHeight += shape->getHeight();
                    
                    contentWidth = max<float>(contentWidth, shape->marginLeft + shape->getWidth() + shape->marginRight);
                    previousMargin = shape->marginBottom;
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
            
            for (vector<ShapeRef>::const_iterator it = components.begin(); it != components.end(); ++it)
            {
                ShapeRef shape = *it;
                
                if (shape->visible)
                {
                    float left = paddingLeft;
                    
                    switch (alignX)
                    {
                        case ALIGN_LEFT:
                            left += shape->marginLeft;
                            break;
                            
                        case ALIGN_MIDDLE:
                            left += (innerWidth - shape->getWidth()) * 0.5f; // XXX
                            break;
                            
                        case ALIGN_RIGHT:
                            left += innerWidth - shape->getWidth() - shape->marginRight;
                            break;
                    }
                    
                    top += mergedMargin(previousMargin, shape->marginTop);
                    shape->setLocation(x + left, y + top);
                    
                    top += shape->getHeight();
                    previousMargin = shape->marginBottom;
                }
            }
        }
        
        layoutRequest = false;
    }
}
