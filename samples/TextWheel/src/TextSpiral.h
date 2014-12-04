/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/font/zf/VirtualFont.h"

class TextSpiral
{
public:
    float ox;
    float oy;
    float r1;
    float r2;
    float turns;
    
    std::vector<ci::Vec2f> vertices;

    void update(float ox, float oy, float r1, float r2, float turns, float DD1, float DD2);
    void drawWire();
    void drawText(chr::ZFont &font, const chr::zf::LineLayout &layout, float offsetX = 0, float offsetY = 0);
};
