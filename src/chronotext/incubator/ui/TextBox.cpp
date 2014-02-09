/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/incubator/ui/TextBox.h"

#include <limits>

using namespace std;
using namespace ci;

namespace chronotext
{
    TextBox::TextBox()
    :
    Shape()
    {}

    TextBox::TextBox(shared_ptr<Style> style)
    :
    Shape(style),
    font(NULL),
    fontSize(0),
    wrap(false),
    lineHeight(0),
    lineHeightFactor(0),
    overflowX(false),
    overflowY(false),
    offsetX(0),
    offsetY(0),
    textColor(style->textColor),
    fill(style->fill),
    updateWrapRequest(false),
    updateWidthRequest(false),
    updateHeightRequest(false),
    updateLineHeightRequest(false)
    {
        setFont(style->font);
        setFontSize(style->fontSize);
        setWrap(style->wrap);
        setTextAlign(style->textAlignX, style->textAlignY);
        
        if (style->lineHeight > 0)
        {
            setLineHeight(style->lineHeight);
        }
        else if (style->lineHeightFactor > 0)
        {
            setLineHeightFactor(style->lineHeightFactor);
        }
    }
    
    void TextBox::setFont(XFont *font)
    {
        if (font != this->font)
        {
            updateLineHeightRequest = true;
            updateWrapRequest = true;
            this->font = font;
        }
    }
    
    void TextBox::setFontSize(float size)
    {
        if (size != fontSize)
        {
            updateLineHeightRequest = true;
            updateWrapRequest = true;
            fontSize = size;
        }
    }
    
    void TextBox::setLineHeightFactor(float factor)
    {
        if (factor != lineHeightFactor)
        {
            updateLineHeightRequest = true;
            lineHeightFactor = factor;
        }
    }
    
    void TextBox::setLineHeight(float height)
    {
        if (height != lineHeight)
        {
            updateLineHeightRequest = true;
            lineHeightFactor = 0;
            lineHeight = height;
        }
    }
    
    void TextBox::setWidth(float width)
    {
        if (autoWidth || width != this->width)
        {
            updateWidthRequest = true;
            updateWrapRequest = true;
            Shape::setWidth(width);
        }
    }
    
    void TextBox::setHeight(float height)
    {
        if (autoHeight || height != this->height)
        {
            updateHeightRequest = true;
            Shape::setHeight(height);
        }
    }
    
    void TextBox::setAutoWidth(bool autoWidth)
    {
        if (autoWidth != this->autoWidth)
        {
            updateWrapRequest = true;
            updateWidthRequest = true;
            Shape::setAutoWidth(autoWidth);
        }
    }
    
    void TextBox::setAutoHeight(bool autoHeight)
    {
        if (autoHeight != this->autoHeight)
        {
            updateHeightRequest = true;
            Shape::setAutoHeight(autoHeight);
        }
    }
    
    void TextBox::setPadding(float left, float top, float right, float bottom)
    {
        if (left != paddingLeft || right != paddingRight)
        {
            updateWidthRequest = true;
            updateWrapRequest = true;
        }
        if (top != paddingTop || bottom != paddingBottom)
        {
            updateHeightRequest = true;
        }
        
        Shape::setPadding(left, top, right, bottom);
    }
    
    void TextBox::setTextAlign(int x, int y)
    {
        textAlignX = x;
        textAlignY = y;
    }
    
    void TextBox::setWrap(bool wrap)
    {
        if (wrap != this->wrap)
        {
            updateWrapRequest = true;
            requestContainerLayout();
            this->wrap = wrap;
        }
    }
    
    void TextBox::setOffsetX(float x)
    {
        offsetX = x;
    }
    
    void TextBox::setOffsetY(float y)
    {
        offsetY = y;
    }
    
    void TextBox::setText(const wstring &text)
    {
        this->text = text;
        updateWrapRequest = true;
    }
    
    float TextBox::getWidth()
    {
        layout();
        return width;
    }
    
    float TextBox::getHeight()
    {
        layout();
        return height;
    }
    
    float TextBox::getLineHeight()
    {
        layout();
        return lineHeight;
    }
    
    void TextBox::draw()
    {
        layout();
        fill.draw(getBounds());
        
        /*
         * TODO: BEGIN CLIP
         */
        
        drawText();
        
        /*
         * TODO: END CLIP
         */
    }
    
    /*
     * AN index EQUAL TO numeric_limits<int>::min() MEANS: FIRST CHARACTER ON THE LINE
     * AN index EQUAL TO numeric_limits<int>::max() MEANS: LAST CHARACTER ON THE LINE
     */
    Vec2f TextBox::getLocationAt(int line, int index)
    {
        int start = wrapper.offsets[line];
        int end = start + wrapper.lengths[line];
        
        if (index == numeric_limits<int>::min())
        {
            index = start;
        }
        else if (index == numeric_limits<int>::max())
        {
            index = end;
        }
        
        float xx = x + paddingLeft + getOffsetX(start, end) + font->getSubStringAdvance(text, start, index);
        float yy = y + paddingTop + getOffsetY() + line * lineHeight;
        
        return Vec2f(xx, yy);
    }
    
    float TextBox::getOffsetX(int start, int end)
    {
        float ox = offsetX;
        
        switch (textAlignX)
        {
            case ALIGN_MIDDLE :
                ox += (width - paddingLeft - paddingRight - font->getSubStringAdvance(text, start, end)) / 2;
                break;
                
            case ALIGN_RIGHT :
                ox += width - paddingLeft - paddingRight - font->getSubStringAdvance(text, start, end);
                break;
        }
        
        return ox;
    }
    
    float TextBox::getOffsetY()
    {
        float oy = offsetY;
        
        switch (textAlignY)
        {
            case ALIGN_MIDDLE :
                oy += (height - paddingTop - paddingBottom - contentHeight) / 2;
                break;
                
            case ALIGN_BOTTOM :
                oy += height - paddingTop - paddingBottom - contentHeight;
                break;
        }
        
        return oy;
    }
    
    float TextBox::getLineTop(int index)
    {
        return index * lineHeight;
    }
    
    /*
     * ASSERTION: n > 0
     */
    float TextBox::getLinesHeight(int n)
    {
        return getLineTop(n - 1) + font->getHeight();
    }
    
    void TextBox::layout()
    {
        font->setSize(fontSize);
        
        if (updateLineHeightRequest && lineHeightFactor > 0)
        {
            lineHeight = font->getHeight() * lineHeightFactor;
        }
        
        if (updateWrapRequest)
        {
            if (wrap && !autoWidth)
            {
                wrapper.wrap(font, text, width - paddingLeft - paddingRight);
            }
            else
            {
                contentWidth = wrapper.wrap(font, text);
            }
        }
        
        if (updateWidthRequest || updateWrapRequest)
        {
            if (autoWidth)
            {
                width = paddingLeft + contentWidth + paddingRight;
            }
            overflowX = !autoWidth && (contentWidth > (width - paddingLeft - paddingRight));
        }
        
        if (updateHeightRequest || updateWrapRequest || updateLineHeightRequest)
        {
            contentHeight = getLinesHeight(wrapper.size);
            
            if (autoHeight)
            {
                height = paddingTop + contentHeight + paddingBottom;
            }
            overflowY = !autoHeight && (contentHeight > (height - paddingTop - paddingBottom));
        }
        
        updateLineHeightRequest = false;
        updateWrapRequest = false;
        updateWidthRequest = false;
        updateHeightRequest = false;
    }
    
    void TextBox::drawText()
    {
        gl::color(textColor);
        
        float innerWidth = width - paddingLeft - paddingRight;
        float innerHeight = height - paddingTop - paddingBottom;
        
        float limitTop = y + paddingTop + font->getAscent();
        float limitBottom = limitTop + innerHeight;
        float yy = limitTop + getOffsetY();
        
        font->beginSequence(NULL, 2);
        
        for (int i = 0; i < wrapper.size && yy < limitBottom; i++)
        {
            if (yy + lineHeight > limitTop)
            {
                int start = wrapper.offsets[i];
                int end = start + wrapper.lengths[i];
                
                float limitLeft = x + paddingLeft;
                float limitRight = limitLeft + innerWidth;
                float xx = limitLeft + getOffsetX(start, end);
                
                drawTextSpan(xx, yy, start, end, limitLeft, limitRight);
            }
            
            yy += lineHeight;
        }
        
        font->endSequence();
    }
    
    void TextBox::drawTextSpan(float xx, float yy, int start, int end, float limitLeft, float limitRight)
    {
        while (start < end && xx < limitRight)
        {
            int cc = font->getGlyphIndex(text[start++]);
            float ww = font->getGlyphAdvance(cc);
            
            if (xx + ww > limitLeft)
            {
                font->addGlyph(cc, xx, yy);
            }
            
            xx += ww;
        }
    }
}
