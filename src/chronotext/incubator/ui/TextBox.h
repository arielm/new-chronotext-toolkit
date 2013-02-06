/*
 * BASED ON TextBox.java FROM GRAPH PROJECT
 *
 * TODO:
 * - IMPLEMENT CLIPPING
 * - FILL-COLOR AND BORDER-COLOR
 */

#pragma once

#include "chronotext/incubator/ui/Shape.h"
#include "chronotext/incubator/ui/TextBoxStyle.h"
#include "chronotext/text/WordWrapper.h"

namespace chronotext
{
    typedef boost::shared_ptr<class TextBox> TextBoxRef;

    class TextBox : public Shape
    {
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
        
    public:
        chr::XFont *font;
        float fontSize;
        bool wrap;

        float lineHeight;
        float lineHeightFactor;

        int textAlignX;
        int textAlignY;

        ci::ColorA textColor;
        ci::ColorA backgroundColor;

        bool overflowX;
        bool overflowY;
        float offsetX;
        float offsetY;
        
        std::wstring text;
        WordWrapper wrapper;

        TextBox() : Shape() {}
        TextBox(TextBoxStyleRef style);
        
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
        void setTextColor(const ci::ColorA &color);
        void setBackgroundColor(const ci::ColorA &color);
        virtual void setText(const std::wstring &text);
        
        float getWidth();
        float getHeight();
        float getLineHeight();
        
        void layout();
        void draw();
        
        ci::Vec2f getLocationAt(int line, int index);
    };
}

namespace chr = chronotext;
