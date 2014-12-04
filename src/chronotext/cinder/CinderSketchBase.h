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
        enum
        {
            FLAG_APP_SHOWN,
            FLAG_APP_HIDDEN,
            FLAG_APP_RESUMED,
            FLAG_APP_PAUSED
        };
        
        enum
        {
            EVENT_FOREGROUND,
            EVENT_BACKGROUND,
            EVENT_MEMORY_WARNING,
            EVENT_CONTEXT_LOST,
            EVENT_CONTEXT_RENEWED,
            EVENT_BACK_KEY
        };
        
        enum
        {
            ACTION_CAPTURE_BACK_KEY = 1,
            ACTION_RELEASE_BACK_KEY
        };
        
        CinderSketchBase() : SuperHandler() {}
        virtual ~CinderSketchBase() {}
        
        virtual bool init() { return true; }
        virtual void setup() {}
        virtual void shutdown() {}

        virtual void resize() {}
        virtual void event(int eventId) {}
        
        virtual void start(int flags) {}
        virtual void stop(int flags) {}

        virtual void update() {}
        virtual void draw() {}
        
        virtual void addTouch(int index, float x, float y) {}
        virtual void updateTouch(int index, float x, float y) {}
        virtual void removeTouch(int index, float x, float y) {}
        
        virtual void accelerated(AccelEvent event) {}
        virtual void enableAccelerometer(float updateFrequency = 30, float filterFactor = 0.1f) {}
        virtual void disableAccelerometer() {}
        
        virtual chr::FrameClock& clock() const = 0;
        virtual ci::Timeline& timeline() const = 0;

        virtual double getElapsedSeconds() const = 0;
        virtual uint32_t getElapsedFrames() const = 0;

        virtual bool isEmulated() const = 0;
        virtual DisplayInfo displayInfo() const = 0;
        virtual WindowInfo windowInfo() const = 0;

        virtual ci::Vec2i getWindowSize() const { return windowInfo().getSize(); }
        virtual int getWindowWidth() const { return windowInfo().getWidth(); };
        virtual int getWindowHeight() const { return windowInfo().getHeight(); };
        virtual ci::Area getWindowBounds() const { return windowInfo().getBounds(); };
        virtual ci::Vec2f getWindowCenter() const { return windowInfo().getCenter(); };
        virtual float getWindowAspectRatio() const { return windowInfo().getAspectRatio(); };
        
        virtual void action(int actionId) = 0;
        virtual void sendMessageToDelegate(int what, const std::string &body = "") = 0;
    };
}
