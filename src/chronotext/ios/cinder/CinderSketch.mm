/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/ios/cinder/CinderSketch.h"

#import "CinderDelegate.h"

using namespace std;
using namespace ci;
using namespace app;

namespace chronotext
{
    static CinderDelegate* getContext(void *context)
    {
        return static_cast<CinderDelegate*>(context);
    }
    
    static CinderDelegate* getDelegate(void *delegate)
    {
        return static_cast<CinderDelegate*>(delegate);
    }
    
    CinderSketch::CinderSketch()
    :
    CinderSketchBase(),
    context(nullptr),
    delegate(nullptr),
    mClock(new FrameClock()),
    mTimeline(Timeline::create())
    {}
    
#pragma mark ---------------------------------------- GETTERS ----------------------------------------

    
    boost::asio::io_service& CinderSketch::io_service() const
    {
        return *(getContext(context).io);
    }
    
    double CinderSketch::getElapsedSeconds() const
    {
        return getContext(context).elapsedSeconds;
    }
    
    uint32_t CinderSketch::getElapsedFrames() const
    {
        return getContext(context).elapsedFrames;
    }
    
    bool CinderSketch::isEmulated() const
    {
        return getContext(context).emulated;
    }
    
    DisplayInfo CinderSketch::getDisplayInfo() const
    {
        return getContext(context).displayInfo;
    }
    
    WindowInfo CinderSketch::getWindowInfo() const
    {
        return getContext(context).windowInfo;
    }

#pragma mark ---------------------------------------- ACCELEROMETER ----------------------------------------
    
    void CinderSketch::enableAccelerometer(float updateFrequency, float filterFactor)
    {
        getContext(context).accelFilter = AccelEvent::Filter(filterFactor);
        
        if (updateFrequency <= 0)
        {
            updateFrequency = 30;
        }
        
        [[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1 / updateFrequency)];
        [[UIAccelerometer sharedAccelerometer] setDelegate:getContext(context)];
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
    
#pragma mark ---------------------------------------- ACTIONS AND MESSAGES ----------------------------------------
    
    void CinderSketch::action(int actionId)
    {
        if (delegate)
        {
            [getDelegate(delegate) action:actionId];
        }
    }
    
    void CinderSketch::sendMessageToDelegate(int what, const string &body)
    {
        if (delegate)
        {
            if (body.empty())
            {
                [getDelegate(delegate) receiveMessageFromSketch:what body:nil];
            }
            else
            {
                [getDelegate(delegate) receiveMessageFromSketch:what body:[NSString stringWithUTF8String:body.data()]];
            }
        }
    }
}
