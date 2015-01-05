/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/cinder/WindowInfo.h"
#include "chronotext/os/SuperHandler.h"
#include "chronotext/system/DisplayInfo.h"
#include "chronotext/time/FrameClock.h"
#include "chronotext/utils/accel/AccelEvent.h"

#include "cinder/Timeline.h"

namespace chr
{
    class CinderSketchBase : public SuperHandler
    {
    public:
        enum Reason
        {
            REASON_APP_SHOWN,
            REASON_APP_HIDDEN,
            REASON_APP_RESUMED,
            REASON_APP_PAUSED
        };
        
        enum Event
        {
            EVENT_FOREGROUND,
            EVENT_BACKGROUND,
            EVENT_MEMORY_WARNING,
            EVENT_CONTEXT_LOST,
            EVENT_CONTEXT_RENEWED,
            EVENT_BACK_KEY
        };
        
        virtual ~CinderSketchBase() {}
        
        virtual bool init() { return true; }
        virtual void setup() {}
        virtual void shutdown() {}

        virtual void resize() {}
        virtual void event(Event event) {}
        
        virtual void start(Reason reason) {}
        virtual void stop(Reason reason) {}

        virtual void update() {}
        virtual void draw() {}
        
        virtual void addTouch(int index, float x, float y) {}
        virtual void updateTouch(int index, float x, float y) {}
        virtual void removeTouch(int index, float x, float y) {}
        
        virtual void accelerated(AccelEvent event) {}
        virtual void enableAccelerometer(float updateFrequency = 30, float filterFactor = 0.1f) {}
        virtual void disableAccelerometer() {}
        
        virtual chr::FrameClock::Ref clock() const = 0;
        virtual ci::Timeline& timeline() const = 0;

        virtual double getElapsedSeconds() const = 0;
        virtual uint32_t getElapsedFrames() const = 0;

        virtual bool isEmulated() const = 0;
        virtual const WindowInfo& getWindowInfo() const = 0;

        virtual ci::Vec2i getWindowSize() const { return getWindowInfo().size; }
        virtual int getWindowWidth() const { return getWindowInfo().size.x; };
        virtual int getWindowHeight() const { return getWindowInfo().size.y; };
        virtual ci::Area getWindowBounds() const { return getWindowInfo().bounds(); };
        virtual ci::Vec2f getWindowCenter() const { return getWindowInfo().center(); };
        virtual float getWindowAspectRatio() const { return getWindowInfo().aspectRatio(); };
        
        virtual void action(int actionId) = 0;
        virtual void sendMessageToDelegate(int what, const std::string &body = "") = 0;
    };
}
