/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/cinder/CinderSketchBase.h"

namespace chr
{
    class CinderSketch : public CinderSketchBase
    {
    public:
        double getElapsedSeconds() const final;
        int getElapsedFrames() const final;
        
        bool isEmulated() const final;
        const WindowInfo& getWindowInfo() const final;

        inline chr::FrameClock::Ref clock() const final { return clock_; }
        inline ci::Timeline& timeline() const final { return *timeline_; }

    protected:
        chr::FrameClock::Ref clock_ = chr::FrameClock::create();
        ci::TimelineRef timeline_ = ci::Timeline::create();
    };
}
