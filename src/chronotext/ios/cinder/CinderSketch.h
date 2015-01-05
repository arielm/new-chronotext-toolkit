/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/cinder/CinderSketchBase.h"

#include "cinder/app/TouchEvent.h"

namespace chr
{
    class CinderSketch : public CinderSketchBase
    {
    public:
        CinderSketch();
        
        void* getDelegate() const;
        void setDelegate(void *delegate);

        inline chr::FrameClock::Ref clock() const final { return clock_; }
        inline ci::Timeline& timeline() const final { return *timeline_; }
        
        double getElapsedSeconds() const final;
        uint32_t getElapsedFrames() const final;
        
        bool isEmulated() const final;
        const WindowInfo& getWindowInfo() const final;

        void action(int actionId) final;
        void sendMessageToDelegate(int what, const std::string &body = "") final;

        void enableAccelerometer(float updateFrequency = 30, float filterFactor = 0.1f) final;
        void disableAccelerometer() final;

        void touchesBegan(ci::app::TouchEvent event);
        void touchesMoved(ci::app::TouchEvent event);
        void touchesEnded(ci::app::TouchEvent event);
        
    protected:
        void *delegate;
        
        chr::FrameClock::Ref clock_;
        ci::TimelineRef timeline_;
    };
}
