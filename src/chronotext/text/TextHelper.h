/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/font/xf/Font.h"
#include "chronotext/path/FollowablePath.h"

namespace chronotext
{
    namespace xf
    {
        class TextHelper
        {
        public:
            static void drawText(Font &font, const std::wstring &text, float x = 0, float y = 0);
            static inline void drawText(Font &font, const std::wstring &text, const ci::Vec2f &position = ci::Vec2f::zero()) { drawText(font, text, position.x, position.y); }
            
            static void drawAlignedText(Font &font, const std::wstring &text, const ci::Vec2f &position = ci::Vec2f::zero(), Font::Alignment alignX = Font::ALIGN_MIDDLE, Font::Alignment alignY = Font::ALIGN_MIDDLE);
            static inline void drawAlignedText(Font &font, const std::wstring &text, float x = 0, float y = 0, Font::Alignment alignX = Font::ALIGN_MIDDLE, Font::Alignment alignY = Font::ALIGN_MIDDLE) { drawAlignedText(font, text, ci::Vec2f(x, y), alignX, alignY); }
            
            static void drawTextInRect(Font &font, const std::wstring &text, const ci::Rectf &rect);
            static inline void drawTextInRect(Font &font, const std::wstring &text, float x1, float y1, float x2, float y2) { drawTextInRect(font, text, ci::Rectf(x1, y1, x2, y2)); }
            
            static float drawTextOnPath(Font &font, const std::wstring &text, const FollowablePath &path, float offset = 0);
        };
    }
}

namespace chr = chronotext;
