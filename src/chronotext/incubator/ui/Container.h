#pragma once

#include "chronotext/incubator/ui/Shape.h"

namespace chronotext
{
    typedef boost::shared_ptr<class Container> ContainerRef;

    class Container : public Shape
    {
    protected:
        bool layoutRequest;
        
        float mergedMargin(float previousMargin, float nextMargin);

    public:
        std::vector<ShapeRef> components;

        Container() : Shape() {}

        Container(ShapeStyleRef style)
        :
        Shape(style),
        layoutRequest(false)
        {}
        
        void setWidth(float newWidth);
        void setHeight(float newHeight);
        void setAutoWidth(bool autoWidth);
        void setAutoHeight(bool autoHeight);
        void setPadding(float left, float top, float right, float bottom);
        
        float getWidth();
        float getHeight();
        Shape* getComponentByTag(int tag);

        void addComponent(ShapeRef component);
        void requestLayout();
        
        void draw();
    };
}

namespace chr = chronotext;
