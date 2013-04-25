#pragma once

#include "chronotext/font/XFont.h"
#include "chronotext/path/FollowablePath.h"

class FontHelper
{
public:
    static float getStringWidth(chr::XFont *font, const std::wstring &text, bool snap = false);

    static void drawText(chr::XFont *font, chr::XFontSequence *sequence, const std::wstring &text, float x = 0, float y = 0, bool snap = false);
    static void drawAlignedText(chr::XFont *font, chr::XFontSequence *sequence, const std::wstring &text, float x = 0, float y = 0, int alignX = chr::XFont::ALIGN_MIDDLE, int alignY = chr::XFont::ALIGN_MIDDLE, bool snap = false);
    
    static void drawTextInRect(chr::XFont *font, chr::XFontSequence *sequence, const std::wstring &text, const ci::Rectf &rect, bool snap = false);
    static void drawTextInRect(chr::XFont *font, chr::XFontSequence *sequence, const std::wstring &text, float x1, float y1, float x2, float y2, bool snap = false);
    
    static void drawStrikethroughInRect(chr::XFont *font, const std::wstring &text, const ci::Rectf &rect, bool snap = false);
    static void drawStrikethroughInRect(chr::XFont *font, const std::wstring &text, float x1, float y1, float x2, float y2, bool snap = false);
    
    static float drawTextOnPath(chr::XFont *font, chr::XFontSequence *sequence, const std::wstring &text, FollowablePath *path, float offset);
};
