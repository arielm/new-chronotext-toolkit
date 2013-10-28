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
 * LIMITATIONS:
 * 1) COMPONENTS WITH FIXED-WIDTH RECEIVE THE WIDTH OF THE VerticalLayout
 * 2) COMPONENTS WITH AUTO-WIDTH CAN'T BE LAYED-OUT WHEN THE WIDTH OF THE VerticalLayout IS UNDEFINED
 *
 * TODO:
 * 1) FIX LIMITATION 1
 * 2) HANDLE CONTENT-OVERFLOW
 * 3) LAYOUT CODE COULD BE MORE EFFECTIVE (I.E. NO NEED TO USE setLocation() TWICE)
 */

namespace chronotext
{
    typedef std::shared_ptr<class VerticalLayout> VerticalLayoutRef;
    
    class VerticalLayout : public Container
    {
    public:
        int alignX;
        int alignY;

        VerticalLayout();
        VerticalLayout(LayoutStyleRef style);
        
        void layout();
        
    protected:
        float contentWidth;
        float contentHeight;
    };
}

namespace chr = chronotext;
