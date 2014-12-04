/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/android/cinder/CinderSketch.h"
#include "chronotext/android/cinder/CinderDelegate.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace chronotext
{
    CinderSketch::CinderSketch()
    :
    CinderSketchBase(),
    delegate(nullptr),
    clock_(FrameClock::create()),
    timeline_(Timeline::create())
    {}
    
    CinderDelegate* CinderSketch::getDelegate() const
    {
        return delegate;
    }
    
    void CinderSketch::setDelegate(CinderDelegate *delegate)
    {
        this->delegate = delegate;
    }
    
#pragma mark ---------------------------------------- GETTERS ----------------------------------------

    FrameClock& CinderSketch::clock() const
    {
        return *clock_;
    }
    
    Timeline& CinderSketch::timeline() const
    {
        return *timeline_;
    }
    
    double CinderSketch::getElapsedSeconds() const
    {
        return delegate->getElapsedSeconds();
    }
    
    uint32_t CinderSketch::getElapsedFrames() const
    {
        return delegate->getElapsedFrames();
    }

    bool CinderSketch::isEmulated() const
    {
        return delegate->isEmulated();
    }
    
    DisplayInfo CinderSketch::displayInfo() const
    {
        return delegate->displayInfo();
    }
    
    WindowInfo CinderSketch::windowInfo() const
    {
        return delegate->windowInfo();
    }
    
#pragma mark ---------------------------------------- MESSAGES AND ACTIONS ----------------------------------------
    
    void CinderSketch::action(int actionId)
    {
        delegate->action(actionId);
    }
    
    void CinderSketch::sendMessageToDelegate(int what, const string &body)
    {
        delegate->receiveMessageFromSketch(what, body);
    }
    
#pragma mark ---------------------------------------- ACCELEROMETER ----------------------------------------
    
    void CinderSketch::enableAccelerometer(float updateFrequency, float filterFactor)
    {
        delegate->enableAccelerometer(updateFrequency, filterFactor);
    }
    
    void CinderSketch::disableAccelerometer()
    {
        delegate->disableAccelerometer();
    }
}
