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
    float TextHelper::getStringAdvance(XFont *font, const wstring &text, bool snap)
    {
        if (snap)
        {
            float advance = 0;
            
            for (auto c : text)
            {
                int cc = font->getGlyphIndex(c);
                advance += math<float>::floor(font->getGlyphAdvance(cc));
            }
            
            return advance;
        }
        else
        {
            return font->getStringAdvance(text);
        }
    }
    
    void TextHelper::drawText(XFont *font, XFontSequence *sequence, const wstring &text, float x, float y, bool snap)
    {
        if (snap)
        {
            x = math<float>::floor(x);
            y = math<float>::floor(y);
        }
        
        font->beginSequence(sequence, 2);
        
        for (auto c : text)
        {
            int cc = font->getGlyphIndex(c);
            font->addGlyph(cc, x, y);
            
            float advance = font->getGlyphAdvance(cc) * font->getDirection();
            x += snap ? math<float>::floor(advance) : advance;
        }
        
        font->endSequence();
    }
    
    void TextHelper::drawAlignedText(XFont *font, XFontSequence *sequence, const wstring &text, float x, float y, int alignX, int alignY, bool snap)
    {
        switch (alignX)
        {
            case XFont::ALIGN_MIDDLE:
                x -= getStringAdvance(font, text, snap) * 0.5f;
                break;
                
            case XFont::ALIGN_RIGHT:
                x -= getStringAdvance(font, text, snap);
                break;
        }
        
        switch (alignY)
        {
            case XFont::ALIGN_TOP:
                y += font->getAscent();
                break;
                
            case XFont::ALIGN_MIDDLE:
                y += font->getStrikethroughOffset();
                break;
                
            case XFont::ALIGN_BOTTOM:
                y -= font->getDescent();
                break;
        }
        
        drawText(font, sequence, text, x, y, snap);
    }
    
    void TextHelper::drawTextInRect(XFont *font, XFontSequence *sequence, const wstring &text, const Rectf &rect, bool snap)
    {
        drawTextInRect(font, sequence, text, rect.x1, rect.y1, rect.x2, rect.y2, snap);
    }
    
    void TextHelper::drawTextInRect(XFont *font, XFontSequence *sequence, const wstring &text, float x1, float y1, float x2, float y2, bool snap)
    {
        float w = x2 - x1;
        float x = x1 + (w - getStringAdvance(font, text, snap)) * 0.5f;
        
        float h = y2 - y1;
        float y = y1 + h * 0.5f + font->getStrikethroughOffset();
        
        drawText(font, sequence, text, x, y, snap);
    }
    
    void TextHelper::drawStrikethroughInRect(XFont *font, const wstring &text, const Rectf &rect, bool snap)
    {
        drawStrikethroughInRect(font, text, rect.x1, rect.y1, rect.x2, rect.y2, snap);
    }
    
    void TextHelper::drawStrikethroughInRect(XFont *font, const wstring &text, float x1, float y1, float x2, float y2, bool snap)
    {
        float w1 = getStringAdvance(font, text, snap);
        float w2 = x2 - x1;
        float x3 = x1 + (w2 - w1) * 0.5f;
        float x4 = x3 + w1;
        
        float h = y2 - y1;
        float y3 = y1 + h * 0.5f;
        
        if (snap)
        {
            x3 = math<float>::floor(x3);
            x4 = math<float>::floor(x4);
            y3 = math<float>::floor(y3);
        }
        
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
    
    float TextHelper::drawTextOnPath(XFont *font, XFontSequence *sequence, const wstring &text, FollowablePath *path, float offset)
    {
        float offsetX = offset;
        float offsetY = font->getStrikethroughOffset();
        float sampleSize = font->getSize() * 0.5f;
        
        FontMatrix *matrix = font->getMatrix();
        font->beginSequence(sequence, 2);
        
        for (auto c : text)
        {
            int cc = font->getGlyphIndex(c);
            float half = 0.5f * font->getGlyphAdvance(cc);
            offsetX += half;
            
            if (cc >= 0)
            {
                matrix->setTranslation(path->pos2Point(offsetX));
                matrix->rotateZ(path->pos2SampledAngle(offsetX, sampleSize));
                font->addTransformedGlyph2D(cc, -half, offsetY);
            }
            
            offsetX += half;
        }
        
        font->endSequence();
        
        return offsetX;
    }
}
