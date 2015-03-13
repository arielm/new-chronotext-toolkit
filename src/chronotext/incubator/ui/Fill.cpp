/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "Fill.h"

#include "chronotext/utils/GLUtils.h"

using namespace ci;

namespace chr
{
    Fill::Fill()
    :
    type(TYPE_UNDEFINED)
    {}
    
    Fill::Fill(const ColorA &color)
    :
    type(TYPE_COLOR),
    color(color)
    {}
    
    Fill::Fill(const ColorA &color1, const ColorA &color2, float v1, float v2)
    :
    type(TYPE_GRADIENT),
    color1(color1),
    color2(color2),
    v1(v1),
    v2(v2)
    {}
    
    void Fill::draw(const Rectf &bounds)
    {
        switch (type)
        {
            case TYPE_COLOR:
                if (color.a > 0)
                {
                    gl::color(color);
                    gl::drawSolidRect(bounds);
                }
                break;
                
            case TYPE_GRADIENT:
                utils::gl::drawVerticalGradient(bounds, color1, color2, v1, v2);
                break;
        }
    }
}
