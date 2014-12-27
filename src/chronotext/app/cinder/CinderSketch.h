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
    class CinderDelegate;
    
    class CinderSketch : public CinderSketchBase
    {
    public:
        CinderSketch();
        
        CinderDelegate* getDelegate() const;
        void setDelegate(CinderDelegate *delegate);

        inline chr::FrameClock::Ref clock() const final { return clock_; }
        inline ci::Timeline& timeline() const final { return *timeline_; }
        
        double getElapsedSeconds() const override;
        uint32_t getElapsedFrames() const override;

        bool isEmulated() const override;
        DisplayInfo getDisplayInfo() const override;
        WindowInfo getWindowInfo() const override;

        void action(int actionId) override;
        void sendMessageToDelegate(int what, const std::string &body = "") override;
        
    protected:
        CinderDelegate *delegate;
        
        chr::FrameClock::Ref clock_;
        ci::TimelineRef timeline_;
    };
}
