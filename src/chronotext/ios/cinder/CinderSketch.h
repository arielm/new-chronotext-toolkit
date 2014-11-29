/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
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
        CinderSketch(void *context, void *delegate = nullptr);
        
        chr::FrameClock& clock() const { return *mClock; }
        ci::Timeline& timeline() const { return *mTimeline; }
        
        std::ostream& console() { return std::cout; }
        boost::asio::io_service& io_service() const;
        
        double getElapsedSeconds() const;
        uint32_t getElapsedFrames() const;
        
        bool isEmulated() const;
        DisplayInfo getDisplayInfo() const;
        WindowInfo getWindowInfo() const;

        void enableAccelerometer(float updateFrequency = 30, float filterFactor = 0.1f);
        void disableAccelerometer();

        void touchesBegan(ci::app::TouchEvent event);
        void touchesMoved(ci::app::TouchEvent event);
        void touchesEnded(ci::app::TouchEvent event);

        void action(int actionId);
        void sendMessageToDelegate(int what, const std::string &body = "");
        
    protected:
        void *context;
        void *delegate;
        
        std::shared_ptr<chr::FrameClock> mClock;
        ci::TimelineRef mTimeline;
    };
}
