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

namespace chronotext
{
    class CinderSketch : public CinderSketchBase
    {
    public:
        CinderSketch();
        
        void* getDelegate() const;
        void setDelegate(void *delegate);

        chr::FrameClock& clock() const override;
        ci::Timeline& timeline() const override;
        
        double getElapsedSeconds() const override;
        uint32_t getElapsedFrames() const override;
        
        bool isEmulated() const override;
        DisplayInfo displayInfo() const override;
        WindowInfo windowInfo() const override;

        void action(int actionId) override;
        void sendMessageToDelegate(int what, const std::string &body = "") override;

        void enableAccelerometer(float updateFrequency = 30, float filterFactor = 0.1f) override;
        void disableAccelerometer() override;

        void touchesBegan(ci::app::TouchEvent event);
        void touchesMoved(ci::app::TouchEvent event);
        void touchesEnded(ci::app::TouchEvent event);
        
    protected:
        void *delegate;
        
        std::shared_ptr<chr::FrameClock> clock_;
        ci::TimelineRef timeline_;
    };
}
