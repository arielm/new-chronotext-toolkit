/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * FEATURES:
 *
 * 1) DEMONSTRATES UNICODE TEXT-RENDERING VIA chr::ZFont
 *    - ENGLISH AND HEBREW PROPERLY MIXED, THANKS TO THE "BIDI" ALGORITHM
 *
 * 2) DEMONSTRATES HOW TO DRAW CRISP TEXT:
 *    - FONT-SIZE IS DETERMINED BASED ON THE (MOBILE) SCREEN'S DENSITY
 *
 * 3) DEMONSTRATES HOW TO KEEP THE SAME PHYSICAL FONT-SIZE ACROSS (MOBILE) DEVICES
 *
 *
 * REMINDER: OSX IS USED AS A SIMULATOR:
 * - I.E. THE ACTUAL SCREEN'S DENSITY IS NOT TAKEN IN COUNT
 */

/*
 * IN SYNC WITH https://github.com/arielm/new-chronotext-toolkit/tree/develop
 *
 *
 * ADDITIONAL REQUIREMENTS:
 *
 * 1) hb-icu BLOCK:
 *    https://github.com/arielm/hb-icu/tree/toolkit-4.0.0
 *
 * 2) Freetype BLOCK:
 *    https://github.com/arielm/Freetype/tree/toolkit-4.0.0
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"
#include "chronotext/font/zf/FontManager.h"

class Sketch : public chr::CinderSketch
{
public:
    void setup() final;
    void draw() final;
    
protected:
    chr::zf::FontManager fontManager;
    std::shared_ptr<chr::ZFont> font;
    
    void drawAlignedText(chr::ZFont &font, const std::string &text, const ci::Vec2f &position, chr::ZFont::Alignment alignX, chr::ZFont::Alignment alignY);
};
