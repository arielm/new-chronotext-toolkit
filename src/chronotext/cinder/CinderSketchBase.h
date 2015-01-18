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
#include "chronotext/time/FrameClock.h"
#include "chronotext/utils/accel/AccelEvent.h"

#include "cinder/Timeline.h"
#include "cinder/app/KeyEvent.h"

namespace chr
{
    class CinderDelegate;
    
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
        
        enum
        {
            EVENT_RESUMED = 1,
            EVENT_SHOWN,
            EVENT_PAUSED,
            EVENT_HIDDEN,
            EVENT_FOREGROUND,
            EVENT_BACKGROUND,
            EVENT_MEMORY_WARNING,
            EVENT_CONTEXT_LOST,
            EVENT_CONTEXT_RENEWED,
            EVENT_TRIGGER_BACK,
            EVENT_TRIGGER_ESCAPE
        };
        
        enum
        {
            ACTION_CAPTURE_BACK = 1,
            ACTION_RELEASE_BACK,
            ACTION_CAPTURE_ESCAPE,
            ACTION_RELEASE_ESCAPE,
        };
        
        virtual ~CinderSketchBase() {}
        
        virtual bool init() { return true; }
        virtual void launch() {}
        virtual void setup() {}
        virtual void shutdown() {}

        virtual void event(int eventId) {}

        virtual void resize() {}
        virtual void start(Reason reason) {}
        virtual void stop(Reason reason) {}

        virtual void update() {}
        virtual void draw() {}
        
        virtual void addTouch(int index, float x, float y) {}
        virtual void updateTouch(int index, float x, float y) {}
        virtual void removeTouch(int index, float x, float y) {}
        
        virtual bool keyDown(const ci::app::KeyEvent &event) { return false; }
        virtual bool keyUp(const ci::app::KeyEvent &event) { return false; }

        virtual void accelerated(AccelEvent event) {}

        const WindowInfo& getWindowInfo() const;
        double getElapsedSeconds() const;
        int getElapsedFrames() const;

        FrameClock::Ref clock() const;
        ci::Timeline& timeline() const;

        inline ci::Vec2i getWindowSize() const { return windowInfo.size; }
        inline int getWindowWidth() const { return windowInfo.size.x; };
        inline int getWindowHeight() const { return windowInfo.size.y; };
        inline ci::Area getWindowBounds() const { return windowInfo.bounds(); };
        inline ci::Vec2f getWindowCenter() const { return windowInfo.center(); };
        inline float getWindowAspectRatio() const { return windowInfo.aspectRatio(); };
        
    protected:
        friend class CinderDelegate;
        
        ci::Timer timer;
        int frameCount = 0;
        bool forceResize = false;

        WindowInfo windowInfo;
        FrameClock::Ref _clock = FrameClock::create();
        ci::TimelineRef _timeline = ci::Timeline::create();
        
        void performSetup(const WindowInfo &windowInfo);
        void performResize(const ci::Vec2i &size);
        void performStart(Reason reason);
        void performStop(Reason reason);
        void performUpdate();
    };
}
