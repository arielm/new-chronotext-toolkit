/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/ios/cinder/CinderSketch.h"

#import "CinderDelegate.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace chr
{
    CinderSketch::CinderSketch()
    :
    delegate(nullptr),
    clock_(FrameClock::create()),
    timeline_(Timeline::create())
    {}
    
    void* CinderSketch::getDelegate() const
    {
        return delegate;
    }
    
    void CinderSketch::setDelegate(void *delegate)
    {
        CinderSketch::delegate = delegate;
    }
    
    static inline CinderDelegate* castToDelegate(void *delegate)
    {
        return static_cast<CinderDelegate*>(delegate);
    }

#pragma mark ---------------------------------------- GETTERS ----------------------------------------

    double CinderSketch::getElapsedSeconds() const
    {
        return castToDelegate(delegate).elapsedSeconds;
    }
    
    uint32_t CinderSketch::getElapsedFrames() const
    {
        return castToDelegate(delegate).elapsedFrames;
    }
    
    bool CinderSketch::isEmulated() const
    {
        return castToDelegate(delegate).emulated;
    }
    
    const WindowInfo& CinderSketch::getWindowInfo() const
    {
        return castToDelegate(delegate).windowInfo;
    }
    
#pragma mark ---------------------------------------- MESSAGES AND ACTIONS ----------------------------------------
    
    void CinderSketch::action(int actionId)
    {
        [castToDelegate(delegate) action:actionId];
    }
    
    void CinderSketch::sendMessageToDelegate(int what, const string &body)
    {
        if (body.empty())
        {
            [castToDelegate(delegate) receiveMessageFromSketch:what body:nil];
        }
        else
        {
            [castToDelegate(delegate) receiveMessageFromSketch:what body:[NSString stringWithUTF8String:body.data()]];
        }
    }
    
#pragma mark ---------------------------------------- ACCELEROMETER ----------------------------------------
    
    void CinderSketch::enableAccelerometer(float updateFrequency, float filterFactor)
    {
        castToDelegate(delegate).accelFilter = AccelEvent::Filter(filterFactor);
        
        if (updateFrequency <= 0)
        {
            updateFrequency = 30;
        }
        
        [[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1 / updateFrequency)];
        [[UIAccelerometer sharedAccelerometer] setDelegate:castToDelegate(delegate)];
    }
    
    void CinderSketch::disableAccelerometer()
    {
        [[UIAccelerometer sharedAccelerometer] setDelegate:nil];
    }
    
#pragma mark ---------------------------------------- TOUCH ----------------------------------------

    void CinderSketch::touchesBegan(TouchEvent event)
    {
        for (auto &touch : event.getTouches())
        {
            addTouch(touch.getId() - 1, touch.getX(), touch.getY());
        }
    }
    
    void CinderSketch::touchesMoved(TouchEvent event)
    {
        for (auto &touch : event.getTouches())
        {
            updateTouch(touch.getId() - 1, touch.getX(), touch.getY());
        }
    }
    
    void CinderSketch::touchesEnded(TouchEvent event)
    {
        for (auto &touch : event.getTouches())
        {
            removeTouch(touch.getId() - 1, touch.getX(), touch.getY());
        }
    }
}
