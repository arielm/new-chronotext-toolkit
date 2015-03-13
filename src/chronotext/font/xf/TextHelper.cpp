/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/font/xf/TextHelper.h"

using namespace std;
using namespace ci;

namespace chr
{
    namespace xf
    {
        void TextHelper::drawText(Font &font, const wstring &text, float x, float y)
        {
            font.beginSequence();
            
            for (auto c : text)
            {
                auto glyphIndex = font.getGlyphIndex(c);
                font.addGlyph(glyphIndex, x, y);
                x += font.getGlyphAdvance(glyphIndex) * font.getDirection();
            }
            
            font.endSequence();
        }
        
        void TextHelper::drawAlignedText(Font &font, const wstring &text, const Vec2f &position, Font::Alignment alignX, Font::Alignment alignY)
        {
            drawText(font, text, position + font.getOffset(text, alignX, alignY));
        }
        
        void TextHelper::drawTextInRect(Font &font, const wstring &text, const Rectf &rect)
        {
            drawAlignedText(font, text, rect.getCenter(), Font::ALIGN_MIDDLE, Font::ALIGN_MIDDLE);
        }
        
        void TextHelper::drawTransformedText(Font &font, const std::wstring &text, float x, float y)
        {
            auto direction = font.getDirection();
            auto &matrix = font.getMatrix();
            
            if (x != 0)
            {
                matrix.translate(x * direction, 0);
            }
            
            font.beginSequence();
            
            for (auto c : text)
            {
                auto glyphIndex = font.getGlyphIndex(c);
                font.addTransformedGlyph(glyphIndex, 0, y);
                matrix.translate(font.getGlyphAdvance(glyphIndex) * direction, 0);
            }
            
            font.endSequence();
        }
        
        float TextHelper::drawTextOnPath(Font &font, const wstring &text, const FollowablePath &path, float offsetX, float offsetY)
        {
            auto sampleSize = 0.5f * font.getSize();
            auto direction = font.getDirection();
            auto &matrix = font.getMatrix();
            
            font.beginSequence();
            
            for (auto c : text)
            {
                auto glyphIndex = font.getGlyphIndex(c);
                auto half = 0.5f * font.getGlyphAdvance(glyphIndex) * direction;
                offsetX += half;
                
                if (glyphIndex >= 0)
                {
                    matrix.setTranslation(path.offset2Position(offsetX));
                    matrix.rotateZ(path.offset2SampledAngle(offsetX, sampleSize));
                    font.addTransformedGlyph(glyphIndex, -half, offsetY);
                }
                
                offsetX += half;
            }
            
            font.endSequence();
            return offsetX;
        }
    }
}
