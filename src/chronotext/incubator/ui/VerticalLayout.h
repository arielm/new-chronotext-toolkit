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
 * 2) HANDLE HORIZONTAL AND VERTICAL ALIGNMENT
 * 3) HANDLE CONTENT-OVERFLOW
 */

namespace chronotext
{
    typedef boost::shared_ptr<class VerticalLayout> VerticalLayoutRef;
    
    class VerticalLayout : public Container
    {
    protected:
        float contentWidth;
        float contentHeight;
        
    public:
        int alignX;
        int alignY;

        VerticalLayout() : Container() {}
        
        VerticalLayout(LayoutStyleRef style)
        :
        Container(style),
        alignX(style->alignX),
        alignY(style->alignY)
        {}
        
        void layout();
    };
}

namespace chr = chronotext;
