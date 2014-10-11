/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/cinder/CinderSketchBase.h"

#include "cinder/app/AppNative.h"

namespace chronotext
{
    class CinderApp;
    
    class CinderSketchSimple : public CinderSketchBase
    {
    public:
        CinderSketchSimple(void *context, void *delegate = nullptr);
        
        chr::FrameClock& clock() const { return *mClock; }
        ci::Timeline& timeline() const { return *mTimeline; }
        
        std::ostream& console() { return context->console(); }
        boost::asio::io_service& io_service() const { return context->io_service(); }
        
        double getElapsedSeconds() const { return context->getElapsedSeconds(); }
        uint32_t getElapsedFrames() const { return context->getElapsedFrames(); }

        bool isEmulated() const;
        DisplayInfo getDisplayInfo() const;
        WindowInfo getWindowInfo() const;
        
        void action(int actionId);
        void sendMessageToDelegate(int what, const std::string &body = "");
        
    protected:
        ci::app::AppNative *context;
        CinderApp *delegate;
        
        std::shared_ptr<chr::FrameClock> mClock;
        ci::TimelineRef mTimeline;
    };
}
