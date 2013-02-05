#pragma once

#include "chronotext/incubator/ui/Shape.h"

#include "cinder/Color.h"

namespace chronotext
{
    typedef boost::shared_ptr<ci::ColorAf> ColorRef;
    typedef boost::shared_ptr<class ShapeStyle> ShapeStyleRef;
    
    class ShapeStyle
    {
    public:
        float width;
        float height;
        
        bool autoWidth;
        bool autoHeight;
        
        bool visible;
        
        float paddingLeft;
        float paddingTop;
        float paddingRight;
        float paddingBottom;
        
        float marginLeft;
        float marginTop;
        float marginRight;
        float marginBottom;
        
        ShapeStyle()
        :
        width(0),
        height(0),
        autoWidth(true),
        autoHeight(true),
        visible(true),
        paddingLeft(0),
        paddingTop(0),
        paddingRight(0),
        paddingBottom(0),
        marginLeft(0),
        marginTop(0),
        marginRight(0),
        marginBottom(0)
        {}
        
        void setPadding(float left, float top, float right, float bottom)
        {
            paddingLeft = left;
            paddingTop = top;
            paddingRight = right;
            paddingBottom = bottom;
        }
        
        void setMargin(float left, float top, float right, float bottom)
        {
            marginLeft = left;
            marginTop = top;
            marginRight = right;
            marginBottom = bottom;
        }
    };
}

namespace chr = chronotext;
