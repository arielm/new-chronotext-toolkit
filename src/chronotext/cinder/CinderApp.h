/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * TODO: CHECK IF pause() AND resume() ARE STILL WORKING ON ANDROID
 */

#pragma once

#include "cinder/app/AppNative.h"

#include "chronotext/cinder/CinderSketch.h"

class CinderApp : public ci::app::AppNative
{
public:
    CinderSketch *sketch;
    
    CinderApp()
    :
    startCount(0),
    updateCount(0)
    {}

    void setup();
    void shutdown();
    void resize();

    void update();
    void draw();
    
    void mouseDown(ci::app::MouseEvent event);
    void mouseUp(ci::app::MouseEvent event);
    void mouseDrag(ci::app::MouseEvent event);
    
    void touchesBegan(ci::app::TouchEvent event);
    void touchesMoved(ci::app::TouchEvent event);
    void touchesEnded(ci::app::TouchEvent event);
    
    void accelerated(AccelEvent event);

    virtual void receiveMessageFromSketch(int what, const std::string &body) {}
    void sendMessageToSketch(int what, const std::string &body);
    
#if defined(CINDER_ANDROID)
    void pause();
    void resume(bool renewContext);
#endif
    
protected:
    int startCount;
    int updateCount;
    
    int ticks;
    double t0;
    double elapsed;
    
    void start();
    void stop();
};
