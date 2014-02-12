/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/text/TextHelper.h"

using namespace std;
using namespace ci;

namespace chronotext
{
    void TextHelper::drawText(XFont &font, const wstring &text, float x, float y)
    {
        for (auto c : text)
        {
            auto glyphIndex = font.getGlyphIndex(c);
            font.addGlyph(glyphIndex, x, y);
            x += font.getGlyphAdvance(glyphIndex) * font.getDirection();
        }
    }
    
    void TextHelper::drawAlignedText(XFont &font, const wstring &text, float x, float y, int alignX, int alignY)
    {
        switch (alignX)
        {
            case XFont::ALIGN_MIDDLE:
                x -= font.getStringAdvance(text) * 0.5f;
                break;
                
            case XFont::ALIGN_RIGHT:
                x -= font.getStringAdvance(text);
                break;
        }
        
        switch (alignY)
        {
            case XFont::ALIGN_TOP:
                y += font.getAscent();
                break;
                
            case XFont::ALIGN_MIDDLE:
                y += font.getStrikethroughOffset();
                break;
                
            case XFont::ALIGN_BOTTOM:
                y -= font.getDescent();
                break;
        }
        
        drawText(font, text, x, y);
    }
    
    void TextHelper::drawTextInRect(XFont &font, const wstring &text, const Rectf &rect)
    {
        drawTextInRect(font, text, rect.x1, rect.y1, rect.x2, rect.y2);
    }
    
    void TextHelper::drawTextInRect(XFont &font, const wstring &text, float x1, float y1, float x2, float y2)
    {
        float w = x2 - x1;
        float x = x1 + (w - font.getStringAdvance(text)) * 0.5f;
        
        float h = y2 - y1;
        float y = y1 + h * 0.5f + font.getStrikethroughOffset();
        
        drawText(font, text, x, y);
    }
    
    void TextHelper::drawStrikethroughInRect(XFont &font, const wstring &text, const Rectf &rect)
    {
        drawStrikethroughInRect(font, text, rect.x1, rect.y1, rect.x2, rect.y2);
    }
    
    void TextHelper::drawStrikethroughInRect(XFont &font, const wstring &text, float x1, float y1, float x2, float y2)
    {
        float w1 = font.getStringAdvance(text);
        float w2 = x2 - x1;
        float x3 = x1 + (w2 - w1) * 0.5f;
        float x4 = x3 + w1;
        
        float h = y2 - y1;
        float y3 = y1 + h * 0.5f;
        
        const float vertices[] =
        {
            x3, y3,
            x4, y3
        };
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        glDrawArrays(GL_LINES, 0, 2);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    
    float TextHelper::drawTextOnPath(XFont &font, const wstring &text, const FollowablePath &path, float offset)
    {
        float offsetX = offset;
        float offsetY = font.getStrikethroughOffset();
        float sampleSize = font.getSize() * 0.5f;
        
        auto matrix = font.getMatrix();
        
        for (auto c : text)
        {
            auto glyphIndex = font.getGlyphIndex(c);
            float half = 0.5f * font.getGlyphAdvance(glyphIndex);
            offsetX += half;
            
            if (glyphIndex >= 0)
            {
                matrix->setTranslation(path.pos2Point(offsetX));
                matrix->rotateZ(path.pos2SampledAngle(offsetX, sampleSize));
                font.addTransformedGlyph2D(glyphIndex, -half, offsetY);
            }
            
            offsetX += half;
        }
        
        return offsetX;
    }
}
