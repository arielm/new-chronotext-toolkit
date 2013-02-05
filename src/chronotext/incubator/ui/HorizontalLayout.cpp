#include "chronotext/incubator/ui/HorizontalLayout.h"

using namespace std;
using namespace ci;

namespace chronotext
{
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
            
            for (vector<ShapeRef>::const_iterator it = components.begin(); it != components.end(); ++it)
            {
                ShapeRef shape = *it;
                
                if (shape->visible)
                {
                    contentWidth += mergedMargin(previousMargin, shape->marginLeft);
                    contentWidth += shape->getWidth();
                    
                    contentHeight = fmaxf(contentHeight, shape->marginTop + shape->getHeight() + shape->marginBottom);
                    previousMargin = shape->marginRight;
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
                    left += (innerWidth - contentWidth) * 0.5;
                    break;
                    
                case ALIGN_RIGHT:
                    left += innerWidth - contentWidth;
                    break;
            }
            
            // ---
            
            float innerHeight = height - paddingTop - paddingBottom;
            previousMargin = 0;

            for (vector<ShapeRef>::const_iterator it = components.begin(); it != components.end(); ++it)
            {
                ShapeRef shape = *it;
                
                if (shape->visible)
                {
                    float top = paddingTop;
                    
                    switch (alignY)
                    {
                        case ALIGN_TOP:
                            top += shape->marginTop;
                            break;
                            
                        case ALIGN_MIDDLE:
                            top += (innerHeight - shape->getHeight()) * 0.5; // XXX
                            break;
                            
                        case ALIGN_BOTTOM:
                            top += innerHeight - shape->getHeight() - shape->marginBottom;
                            break;
                    }

                    left += mergedMargin(previousMargin, shape->marginLeft);
                    shape->setLocation(x + left, y + top);
                    
                    left += shape->getWidth();
                    previousMargin = shape->marginRight;
                }
            }
        }
        
        layoutRequest = false;
    }
}
