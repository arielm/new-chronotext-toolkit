/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/font/XFont.h"
#include "chronotext/path/FollowablePath.h"

namespace chronotext
{
    class TextHelper
    {
    public:
        static float getStringWidth(XFont *font, const std::wstring &text, bool snap = false);
        
        static void drawText(XFont *font, XFontSequence *sequence, const std::wstring &text, float x = 0, float y = 0, bool snap = false);
        static void drawAlignedText(XFont *font, XFontSequence *sequence, const std::wstring &text, float x = 0, float y = 0, int alignX = XFont::ALIGN_MIDDLE, int alignY = XFont::ALIGN_MIDDLE, bool snap = false);
        
        static void drawTextInRect(XFont *font, XFontSequence *sequence, const std::wstring &text, const ci::Rectf &rect, bool snap = false);
        static void drawTextInRect(XFont *font, XFontSequence *sequence, const std::wstring &text, float x1, float y1, float x2, float y2, bool snap = false);
        
        static void drawStrikethroughInRect(XFont *font, const std::wstring &text, const ci::Rectf &rect, bool snap = false);
        static void drawStrikethroughInRect(XFont *font, const std::wstring &text, float x1, float y1, float x2, float y2, bool snap = false);
        
        static float drawTextOnPath(XFont *font, XFontSequence *sequence, const std::wstring &text, FollowablePath *path, float offset);
    };
}

namespace chr = chronotext;
