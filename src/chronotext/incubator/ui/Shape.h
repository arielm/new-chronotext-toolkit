#pragma once

#include "chronotext/incubator/ui/Touchable.h"
#include "chronotext/incubator/ui/ShapeStyle.h"

#include "cinder/Vector.h"
#include "cinder/Rect.h"

namespace chronotext
{
    typedef boost::shared_ptr<class Shape> ShapeRef;

    class Container;

    class Shape
    {
    public:
        enum
        {
            ALIGN_LEFT,
            ALIGN_RIGHT,
            ALIGN_TOP,
            ALIGN_BOTTOM,
            ALIGN_MIDDLE
        };
        
        float x;
        float y;

        float width;
        float height;
        
        bool autoWidth;
        bool autoHeight;
        
        float paddingLeft;
        float paddingTop;
        float paddingRight;
        float paddingBottom;

        float marginLeft;
        float marginTop;
        float marginRight;
        float marginBottom;

        bool visible;
        int tag;
        Container *container;
        
        Shape() {}
        Shape(ShapeStyleRef style);
        
        virtual void setLocation(float x, float y);
        virtual void setWidth(float width);
        virtual void setHeight(float height);
        virtual void setAutoWidth(bool autoWidth);
        virtual void setAutoHeight(bool autoHeight);
        virtual void setBounds(const ci::Rectf &bounds);
        virtual void setPadding(float left, float top, float right, float bottom);
        virtual void setMargin(float left, float top, float right, float bottom);
        virtual void setVisible(bool visible);
        
        virtual ci::Vec2f getLocation();
        virtual float getWidth();
        virtual float getHeight();
        virtual ci::Rectf getBounds();
        
        virtual void draw() {};
        
        virtual std::vector<Touchable*> getTouchables();
        virtual void touchStateChanged(Touchable *touchable, int state) {}
        virtual void touchActionPerformed(Touchable *touchable, int action) {}
        
        virtual void setContainer(Container *container);
        virtual void requestContainerLayout();
        virtual void layout() {}
    };
}

namespace chr = chronotext;
