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
        CinderDelegate* getDelegate() const;
        void setDelegate(CinderDelegate *delegate);

        inline chr::FrameClock::Ref clock() const final { return clock_; }
        inline ci::Timeline& timeline() const final { return *timeline_; }
        
        double getElapsedSeconds() const final;
        uint32_t getElapsedFrames() const final;

        bool isEmulated() const final;
        const WindowInfo& getWindowInfo() const final;

        void action(int actionId) final;
        void sendMessageToDelegate(int what, const std::string &body = "") final;
        
        int getCode(const ci::app::KeyEvent &keyEvent) const final;
        bool isShiftDown(const ci::app::KeyEvent &keyEvent) const final;
        bool isAltDown(const ci::app::KeyEvent &keyEvent) const final;
        bool isAccelDown(const ci::app::KeyEvent &keyEvent) const final;
        
    protected:
        CinderDelegate *delegate = nullptr;
        
        chr::FrameClock::Ref clock_ = chr::FrameClock::create();
        ci::TimelineRef timeline_ = ci::Timeline::create();
    };
}
