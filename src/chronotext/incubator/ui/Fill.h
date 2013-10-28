#pragma once

#include "cinder/Color.h"
#include "cinder/Rect.h"

class Fill
{
public:
    enum
    {
        TYPE_UNDEFINED,
        TYPE_COLOR,
        TYPE_GRADIENT
    };
    
    Fill();
    Fill(const ci::ColorA &color);
    Fill(const ci::ColorA &color1, const ci::ColorA &color2, float v1 = 0, float v2 = 1);
    
    void draw(const ci::Rectf &bounds);
    
protected:
    int type;
    ci::ColorA color;
    ci::ColorA color1;
    ci::ColorA color2;
    float v1;
    float v2;
};
