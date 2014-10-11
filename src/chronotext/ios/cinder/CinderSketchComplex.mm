/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/ios/cinder/CinderSketchComplex.h"

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
    
    CinderSketchComplex::CinderSketchComplex(void *context, void *delegate)
    :
    CinderSketchBase(),
    context(context),
    delegate(delegate),
    mClock(new FrameClock()),
    mTimeline(Timeline::create())
    {}
    
    void CinderSketchComplex::touchesBegan(TouchEvent event)
    {
        for (auto &touch : event.getTouches())
        {
            addTouch(touch.getId() - 1, touch.getX(), touch.getY());
        }
    }
    
    void CinderSketchComplex::touchesMoved(TouchEvent event)
    {
        for (auto &touch : event.getTouches())
        {
            updateTouch(touch.getId() - 1, touch.getX(), touch.getY());
        }
    }
    
    void CinderSketchComplex::touchesEnded(TouchEvent event)
    {
        for (auto &touch : event.getTouches())
        {
            removeTouch(touch.getId() - 1, touch.getX(), touch.getY());
        }
    }
    
    void CinderSketchComplex::enableAccelerometer(float updateFrequency, float filterFactor)
    {
        [getContext(context) setAccelFilterFactor:filterFactor];
        
        if (updateFrequency <= 0)
        {
            updateFrequency = 30;
        }
        
        [[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1 / updateFrequency)];
        [[UIAccelerometer sharedAccelerometer] setDelegate:getContext(context)];
    }
    
    void CinderSketchComplex::disableAccelerometer()
    {
        [[UIAccelerometer sharedAccelerometer] setDelegate:nil];
    }
    
    boost::asio::io_service& CinderSketchComplex::io_service() const
    {
        return *(getContext(context)).io;
    }
    
    double CinderSketchComplex::getElapsedSeconds() const
    {
        return getContext(context).elapsedSeconds;
    }
    
    uint32_t CinderSketchComplex::getElapsedFrames() const
    {
        return getContext(context).elapsedFrames;
    }
    
    bool CinderSketchComplex::isEmulated() const
    {
        return getContext(context).simulated;
    }
    
    DisplayInfo CinderSketchComplex::getDisplayInfo() const
    {
        return getContext(context).displayInfo;
    }
    
    WindowInfo CinderSketchComplex::getWindowInfo() const
    {
        return getContext(context).windowInfo;
    }
    
    void CinderSketchComplex::action(int actionId)
    {
        if (delegate)
        {
            [getDelegate(delegate) action:actionId];
        }
    }
    
    void CinderSketchComplex::sendMessageToDelegate(int what, const string &body)
    {
        if (delegate)
        {
            if (body.empty())
            {
                [getDelegate(delegate) receiveMessageFromSketch:what body:nil];
            }
            else
            {
                [getDelegate(delegate) receiveMessageFromSketch:what body:[NSString stringWithUTF8String:body.c_str()]];
            }
        }
    }
}
