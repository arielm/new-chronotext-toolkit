/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/incubator/ui/Container.h"
#include "chronotext/incubator/ui/LayoutStyle.h"

/*
 * TODO:
 * 1) HANDLE CONTENT-OVERFLOW
 */

namespace chronotext
{
    typedef std::shared_ptr<class HorizontalLayout> HorizontalLayoutRef;

    class HorizontalLayout : public Container
    {
    public:
        int alignX;
        int alignY;

        HorizontalLayout();
        HorizontalLayout(LayoutStyleRef style);
        
        void setAlign(int x, int y);
        void layout();
        
    protected:
        float contentWidth;
        float contentHeight;
    };
}

namespace chr = chronotext;
