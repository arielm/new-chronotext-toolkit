#include "chronotext/incubator/ui/VerticalLayout.h"

using namespace std;
using namespace ci;

namespace chronotext
{
    void VerticalLayout::layout()
    {
        if (layoutRequest)
        {
            float innerWidth = width - paddingLeft - paddingRight;
            float left = x + paddingLeft;
            float top = paddingTop;
            
            float previousMargin = 0;
            contentHeight = 0;
            
            for (vector<ShapeRef>::const_iterator it = components.begin(); it != components.end(); ++it)
            {
                ShapeRef shape = *it;
                
                if (shape->visible)
                {
                    top += mergedMargin(previousMargin, shape->marginTop);
                    shape->setLocation(left + shape->marginLeft, y + top);
                    
                    if (!shape->autoWidth)
                    {
                        if (autoWidth)
                        {
                            throw runtime_error("VerticalLayout WITH AUTO-WIDTH MUST CONTAIN COMPONENTS WITH FIXED-WIDTH");
                        }
                        else
                        {
                            float availableWidth = fmaxf(0, innerWidth - shape->marginLeft - shape->marginRight);
                            shape->setWidth(availableWidth);
                        }
                    }
                    
                    contentWidth = fmaxf(contentWidth, shape->marginLeft + shape->getWidth() + shape->marginRight);

                    top += shape->getHeight();
                    previousMargin = shape->marginBottom;
                }
            }
            
            top += previousMargin;
            contentHeight = top - paddingTop;
            
            if (autoWidth)
            {
                width = paddingLeft + contentWidth + paddingRight;
            }
            
            if (autoHeight)
            {
                height = top + paddingBottom;
            }
        }
        
        layoutRequest = false;
    }
}
