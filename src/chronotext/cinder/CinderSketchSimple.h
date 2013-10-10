/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/app/AppNative.h"

#include "chronotext/os/SuperHandler.h"
#include "chronotext/utils/accel/AccelEvent.h"

class CinderApp;

class CinderSketchSimple : public SuperHandler, public Looper
{
protected:
    ci::app::AppNative *context;
    CinderApp *delegate;

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
        EVENT_BACKGROUND
    };

    CinderSketchSimple(void *context, void *delegate = NULL)
    :
    SuperHandler(this),
    context(static_cast<ci::app::AppNative*>(context)),
    delegate(static_cast<CinderApp*>(delegate))
    {}
    
    virtual ~CinderSketchSimple() {};

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
    
    void enableAccelerometer(float updateFrequency = 30, float filterFactor = 0.1f) {}
    void disableAccelerometer() {}
    
    double getElapsedSeconds() const { return context->getElapsedSeconds(); }
    uint32_t getElapsedFrames() const { return context->getElapsedFrames(); }

    int getWindowWidth() const { return context->getWindowWidth(); }
    int getWindowHeight() const { return context->getWindowHeight(); }
    ci::Vec2f getWindowCenter() const { return context->getWindowCenter(); }
    ci::Vec2i getWindowSize() const { return context->getWindowSize(); }
    float getWindowAspectRatio() const { return context->getWindowAspectRatio(); }
    ci::Area getWindowBounds() const { return context->getWindowBounds(); }
    float getWindowContentScale() const { return context->getWindowContentScale(); }
    
    std::ostream& console() { return context->console(); }
    
    void sendMessageToDelegate(int what, const std::string &body = "");
};
