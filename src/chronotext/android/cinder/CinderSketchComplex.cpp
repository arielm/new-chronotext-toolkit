/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/android/cinder/CinderSketchComplex.h"
#include "chronotext/android/cinder/CinderDelegate.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace chronotext
{
    CinderSketchComplex::CinderSketchComplex(void *context, void *delegate)
    :
    CinderSketchBase(),
    context(static_cast<CinderDelegate*>(context)),
    delegate(static_cast<CinderDelegate*>(delegate)),
    mClock(new FrameClock()),
    mTimeline(Timeline::create())
    {}
    
#pragma mark ---------------------------------------- GETTERS ----------------------------------------

    ostream& CinderSketchComplex::console()
    {
        return context->console();
    }
    
    boost::asio::io_service& CinderSketchComplex::io_service() const
    {
        return context->io_service();
    }
    
    double CinderSketchComplex::getElapsedSeconds() const
    {
        return context->getElapsedSeconds();
    }
    
    uint32_t CinderSketchComplex::getElapsedFrames() const
    {
        return context->getElapsedFrames();
    }

    bool CinderSketchComplex::isEmulated() const
    {
        return context->isEmulated();
    }
    
    DisplayInfo CinderSketchComplex::getDisplayInfo() const
    {
        return context->getDisplayInfo();
    }
    
    WindowInfo CinderSketchComplex::getWindowInfo() const
    {
        return context->getWindowInfo();
    }
    
#pragma mark ---------------------------------------- ACCELEROMETER ----------------------------------------

    void CinderSketchComplex::enableAccelerometer(float updateFrequency, float filterFactor)
    {
        context->enableAccelerometer(updateFrequency, filterFactor);
    }
    
    void CinderSketchComplex::disableAccelerometer()
    {
        context->disableAccelerometer();
    }
    
#pragma mark ---------------------------------------- ACTIONS AND MESSAGES ----------------------------------------
    
    void CinderSketchComplex::action(int actionId)
    {
        if (delegate)
        {
            delegate->action(actionId);
        }
    }

    void CinderSketchComplex::sendMessageToDelegate(int what, const string &body)
    {
        if (delegate)
        {
            delegate->receiveMessageFromSketch(what, body);
        }
    }
}
