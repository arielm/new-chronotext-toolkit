/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "FrameClock.h"

using namespace std;

namespace chr
{
    FrameClock::FrameClock()
    :
    Clock(),
    shouldSample(true),
    frameTime(0)
    {}
    
    FrameClock::FrameClock(shared_ptr<TimeBase> timeBase)
    :
    Clock(timeBase),
    shouldSample(true),
    frameTime(0)
    {}
    
    double FrameClock::getTime()
    {
        if (shouldSample)
        {
            frameTime = Clock::getTime();
            shouldSample = false;
        }
        
        return frameTime;
    }
    
    void FrameClock::setTime(double now)
    {
        Clock::setTime(now);
        frameTime = now;
    }
    
    void FrameClock::update()
    {
        shouldSample = true;
    }
}
