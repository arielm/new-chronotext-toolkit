#pragma once

#include "chronotext/incubator/ui/ShapeStyle.h"
#include "chronotext/font/XFont.h"

namespace chronotext
{
    typedef boost::shared_ptr<class TextBoxStyle> TextBoxStyleRef;

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
        
        ci::ColorAf textColor;

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
