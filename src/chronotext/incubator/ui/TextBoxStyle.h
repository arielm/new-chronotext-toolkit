/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/incubator/ui/ShapeStyle.h"
#include "chronotext/incubator/ui/Fill.h"
#include "chronotext/font/XFont.h"

namespace chronotext
{
    typedef std::shared_ptr<class TextBoxStyle> TextBoxStyleRef;

    class TextBoxStyle : public ShapeStyle
    {
    public:
        chr::XFont *font;
        float fontSize;
        bool wrap;
        
        float lineHeight;
        float lineHeightFactor;
        
        int textAlignX;
        int textAlignY;
        
        ci::ColorA textColor;
        Fill fill;

        TextBoxStyle()
        :
        ShapeStyle(),
        font(NULL),
        fontSize(1),
        wrap(true),
        lineHeight(0),
        lineHeightFactor(1),
        textAlignX(Shape::ALIGN_LEFT),
        textAlignY(Shape::ALIGN_TOP)
        {}
    };
}

namespace chr = chronotext;
