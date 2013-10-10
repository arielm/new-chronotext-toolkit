/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/app/AppAndroid.h"

#include "chronotext/os/SuperHandler.h"
#include "chronotext/utils/accel/AccelEvent.h"

class CinderDelegate;

class CinderSketchComplex : public SuperHandler, public Looper
{
protected:
    CinderDelegate *context;
    CinderDelegate *delegate;

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

    CinderSketchComplex(void *context, void *delegate)
    :
    SuperHandler(this),
    context(static_cast<CinderDelegate*>(context)),
    delegate(static_cast<CinderDelegate*>(delegate))
    {}

    virtual ~CinderSketchComplex() {}

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
    
    void enableAccelerometer(float updateFrequency = 30, float filterFactor = 0.1f);
    void disableAccelerometer();
    
    double getElapsedSeconds() const;
    uint32_t getElapsedFrames() const;
    
    int getWindowWidth() const;
    int getWindowHeight() const;
    ci::Vec2f getWindowCenter() const;
    ci::Vec2i getWindowSize() const;
    float getWindowAspectRatio() const;
    ci::Area getWindowBounds() const;
    float getWindowContentScale() const { return 1; }
    
    std::ostream& console();

    void sendMessageToDelegate(int what, const std::string &body = "");
};
