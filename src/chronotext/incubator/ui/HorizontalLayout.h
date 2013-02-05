#pragma once

#include "chronotext/incubator/ui/Container.h"
#include "chronotext/incubator/ui/LayoutStyle.h"

/*
 * TODO:
 * 1) HANDLE CONTENT-OVERFLOW
 */

namespace chronotext
{
    typedef boost::shared_ptr<class HorizontalLayout> HorizontalLayoutRef;

    class HorizontalLayout : public Container
    {
    protected:
        float contentWidth;
        float contentHeight;

    public:
        int alignX;
        int alignY;

        HorizontalLayout() : Container() {}

        HorizontalLayout(LayoutStyleRef style)
        :
        Container(style),
        alignX(style->alignX),
        alignY(style->alignY)
        {}
        
        void setAlign(int x, int y);

        void layout();
    };
}

namespace chr = chronotext;
