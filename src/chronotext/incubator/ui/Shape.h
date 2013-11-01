/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/incubator/ui/Touchable.h"
#include "chronotext/incubator/ui/Fill.h"

#include "cinder/Vector.h"
#include "cinder/Rect.h"
#include "cinder/Color.h"

namespace chronotext
{
    typedef std::shared_ptr<class Shape> ShapeRef;

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
        
        class Style
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
            
            int alignX;
            int alignY;
            
            Fill fill;
            
            Style()
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
            marginBottom(0),
            alignX(ALIGN_MIDDLE),
            alignY(ALIGN_MIDDLE)
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
        Shape(std::shared_ptr<Style> style);
        
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
        virtual void touchStateChanged(Touchable *touchable, int nextState, int prevState) {}
        virtual void touchActionPerformed(Touchable *touchable, int action) {}
        
        virtual void setContainer(Container *container);
        virtual void requestContainerLayout();
        virtual void layout() {}
    };
}

namespace chr = chronotext;
