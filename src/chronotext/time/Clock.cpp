/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * INSPIRED BY javax.media.Clock
 * http://docs.oracle.com/javame/config/cdc/opt-pkgs/api/jsr927/javax/media/Clock.html
 */

#include "chronotext/time/Clock.h"

using namespace std;

namespace chronotext
{
    Clock::Clock()
    :
    timeBase(new DefaultTimeBase()),
    mst(0),
    rate(1),
    state(STOPPED)
    {}
    
    Clock::Clock(shared_ptr<TimeBase> timeBase)
    :
    timeBase(timeBase),
    mst(0),
    rate(1),
    state(STOPPED)
    {
        start();
    }
    
    void Clock::start()
    {
        tbst = timeBase->getTime();
        state = STARTED;
    }
    
    void Clock::stop()
    {
        if (state != STOPPED)
        {
            mst = getTime();
            state = STOPPED;
        }
    }
    
    double Clock::getTime()
    {
        return mst + ((state == STOPPED) ? 0 : (timeBase->getTime() - tbst) * rate);
    }
    
    void Clock::setTime(double now)
    {
        if (state == STOPPED)
        {
            mst = now;
        }
        else
        {
            throw;
        }
    }
    
    int Clock::getState()
    {
        return state;
    }
    
    void Clock::setRate(double factor)
    {
        if (state == STOPPED)
        {
            rate = factor;
        }
        else
        {
            throw;
        }
    }
    
    void Clock::restart()
    {
        stop();
        setTime(0);
        start();
    }
}
