/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/cinder/CinderSketchBase.h"

#include "cinder/app/AppNative.h"

namespace chr
{
    class CinderSketch : public CinderSketchBase
    {
    public:
        bool isEmulated() const final;
        const WindowInfo& getWindowInfo() const final;

        double getElapsedSeconds() const final;
        int getElapsedFrames() const final;
        
        inline chr::FrameClock::Ref clock() const final { return clock_; }
        inline ci::Timeline& timeline() const final { return *timeline_; }

        int getCode(const ci::app::KeyEvent &keyEvent) const final;
        bool isShiftDown(const ci::app::KeyEvent &keyEvent) const final;
        bool isAltDown(const ci::app::KeyEvent &keyEvent) const final;
        bool isAccelDown(const ci::app::KeyEvent &keyEvent) const final;
        
    protected:
        chr::FrameClock::Ref clock_ = chr::FrameClock::create();
        ci::TimelineRef timeline_ = ci::Timeline::create();
    };
}
