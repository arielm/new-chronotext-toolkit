/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/incubator/ui/Shape.h"
#include "chronotext/text/WordWrapper.h"

namespace chronotext
{
    typedef std::shared_ptr<class TextBox> TextBoxRef;

    class TextBox : public Shape
    {
    public:
        class Style : public Shape::Style
        {
        public:
            chr::XFont *font;
            float fontSize;
            bool wrap;
            
            float lineHeight;
            float lineHeightFactor;
            
            int textAlignX;
            int textAlignY;
            
            ci::ColorA textColor;
            
            Style()
            :
            Shape::Style(),
            font(NULL),
            fontSize(1),
            wrap(true),
            lineHeight(0),
            lineHeightFactor(1),
            textAlignX(Shape::ALIGN_LEFT),
            textAlignY(Shape::ALIGN_TOP)
            {}
        };
        
        chr::XFont *font;
        float fontSize;
        bool wrap;

        float lineHeight;
        float lineHeightFactor;

        int textAlignX;
        int textAlignY;

        ci::ColorA textColor;
        Fill fill;

        bool overflowX;
        bool overflowY;
        float offsetX;
        float offsetY;
        
        std::wstring text;
        WordWrapper wrapper;

        TextBox();
        TextBox(std::shared_ptr<Style> style);
        
        void setFont(XFont *font);
        void setFontSize(float size);
        void setLineHeightFactor(float factor);
        void setLineHeight(float height);
        void setWidth(float width);
        void setHeight(float height);
        void setAutoWidth(bool autoWidth);
        void setAutoHeight(bool autoHeight);
        void setPadding(float left, float top, float right, float bottom);
        void setTextAlign(int x, int y);
        void setWrap(bool wrap);
        void setOffsetX(float x);
        void setOffsetY(float y);
        virtual void setText(const std::wstring &text);
        
        float getWidth();
        float getHeight();
        float getLineHeight();
        
        void layout();
        void draw();
        
        ci::Vec2f getLocationAt(int line, int index);
        
    protected:
        bool updateWrapRequest;
        bool updateWidthRequest;
        bool updateHeightRequest;
        bool updateLineHeightRequest;
        
        float contentWidth;
        float contentHeight;
        
        float getOffsetX(int start, int end);
        float getOffsetY();
        float getLineTop(int index);
        float getLinesHeight(int n);
        
        void drawText();
        void drawTextSpan(float xx, float yy, int start, int end, float limitLeft, float limitRight);
    };
}

namespace chr = chronotext;
