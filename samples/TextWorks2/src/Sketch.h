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
 * 1) DEMONSTRATES HOW TO USE ZFont's TAGGING-SYSTEM:
 *    - THE SYSTEM IS PURPOSELY GENERIC AND LOW-LEVEL
 *      I.E. IT IS NOT ONLY FOR STYLING PURPOSES
 *
 * 2) DEMONSTRATES USAGE OF THE TAGGING-SYSTEM FOR STYLING PURPOSES:
 *    - THE StyledLineLayout CLASS ALLOWS TO CREATE A COMPOSITE
 *      LINE-LAYOUT USING DIFFERENT FONTS AND COLORS
 */

/*
 * IN SYNC WITH https://github.com/arielm/new-chronotext-toolkit/tree/v4.0.0
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

#include "StyledLineLayout.h"

class Sketch : public chr::CinderSketch
{
public:
    void setup() final;
    void resize() final;
    void draw() final;
    
protected:
    chr::zf::FontManager fontManager;
    
    std::shared_ptr<chr::ZFont> font1;
    std::shared_ptr<chr::ZFont> font2;
    
    float fontSize;
    std::map<int, StyledLineLayout::Style> styleSheet;
    StyledLineLayout layout;
    
    void drawAlignedText(const StyledLineLayout &layout, const ci::Vec2f &position, chr::ZFont::Alignment alignX, chr::ZFont::Alignment alignY);
};
