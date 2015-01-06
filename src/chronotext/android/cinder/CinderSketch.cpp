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

namespace chr
{
    CinderDelegate* CinderSketch::getDelegate() const
    {
        return delegate;
    }
    
    void CinderSketch::setDelegate(CinderDelegate *delegate)
    {
        CinderSketch::delegate = delegate;
    }
    
#pragma mark ---------------------------------------- GETTERS ----------------------------------------

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
    
    const WindowInfo& CinderSketch::getWindowInfo() const
    {
        return delegate->getWindowInfo();
    }
    
#pragma mark ---------------------------------------- SKETCH <-> DELEGATE COMMUNICATION ----------------------------------------
    
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
