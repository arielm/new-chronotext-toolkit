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
    namespace xf
    {
        void TextHelper::drawText(Font &font, const wstring &text, float x, float y)
        {
            font.beginSequence(NULL, 2);
            
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
            drawAlignedText(font, text, rect.getCenter());
        }
        
        float TextHelper::drawTextOnPath(Font &font, const wstring &text, const FollowablePath &path, float offset)
        {
            float offsetX = offset;
            float offsetY = font.getStrikethroughOffset();
            float sampleSize = font.getSize() * 0.5f;
            
            auto matrix = font.getMatrix();
            font.beginSequence(NULL, 2);
            
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
            
            font.endSequence();
            return offsetX;
        }
    }
}
