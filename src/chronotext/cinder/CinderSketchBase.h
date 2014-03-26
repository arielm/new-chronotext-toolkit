/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/cinder/WindowInfo.h"
#include "chronotext/os/SuperHandler.h"
#include "chronotext/utils/accel/AccelEvent.h"
#include "chronotext/time/FrameClock.h"

#include <boost/asio.hpp>

namespace chronotext
{
    class CinderSketchBase : public SuperHandler
    {
    public:
        enum
        {
            FLAG_FOCUS_GAINED,
            FLAG_FOCUS_LOST,
            FLAG_APP_RESUMED,
            FLAG_APP_PAUSED,
            FLAG_SCREEN_ENTERED,
            FLAG_SCREEN_LEFT
        };
        
        enum
        {
            EVENT_FOREGROUND,
            EVENT_BACKGROUND,
            EVENT_MEMORY_WARNING,
            EVENT_CONTEXT_LOST
        };
        
        CinderSketchBase() : SuperHandler() {}
        virtual ~CinderSketchBase() {}
        
        virtual void setup(bool renewContext) {}
        virtual void shutdown() {}
        virtual void resize() {}
        
        virtual void update() {}
        virtual void draw() {}
        
        virtual void start(int flags) {}
        virtual void stop(int flags) {}
        virtual void event(int id) {}
        
        virtual void addTouch(int index, float x, float y) {}
        virtual void updateTouch(int index, float x, float y) {}
        virtual void removeTouch(int index, float x, float y) {}
        
        virtual void accelerated(AccelEvent event) {}
        virtual void enableAccelerometer(float updateFrequency = 30, float filterFactor = 0.1f) {}
        virtual void disableAccelerometer() {}
        
        virtual std::ostream& console() = 0;
        virtual boost::asio::io_service& io_service() const = 0;
        
        virtual double getElapsedSeconds() const = 0;
        virtual uint32_t getElapsedFrames() const = 0;
        
        virtual int getWindowWidth() const = 0;
        virtual int getWindowHeight() const = 0;
        virtual ci::Vec2f getWindowCenter() const = 0;
        virtual ci::Vec2i getWindowSize() const = 0;
        virtual float getWindowAspectRatio() const = 0;
        virtual ci::Area getWindowBounds() const = 0;
        virtual float getWindowContentScale() const = 0;
        virtual WindowInfo getWindowInfo() const = 0;
        
        virtual chr::FrameClock& clock() const = 0;
        
        virtual void sendMessageToDelegate(int what, const std::string &body = "") = 0;
    };
}

namespace chr = chronotext;
