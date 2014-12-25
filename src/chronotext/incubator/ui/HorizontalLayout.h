/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/incubator/ui/Container.h"

/*
 * TODO:
 * 1) HANDLE CONTENT-OVERFLOW
 */

namespace chr
{
    typedef std::shared_ptr<class HorizontalLayout> HorizontalLayoutRef;

    class HorizontalLayout : public Container
    {
    public:
        int alignX;
        int alignY;
        
        Fill fill;

        HorizontalLayout();
        HorizontalLayout(std::shared_ptr<Shape::Style> style);
        
        void setAlign(int x, int y);
        
        void layout();
        void draw();
        
    protected:
        float contentWidth;
        float contentHeight;
    };
}
