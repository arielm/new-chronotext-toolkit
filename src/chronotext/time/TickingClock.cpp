/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "TickingClock.h"

using namespace std;

namespace chronotext
{
    double TickingClock::getTime()
    {
        return tickTime;
    }
    
    void TickingClock::setTime(int now)
    {
        MasterClock::setTime(now);
        tickTime = now;
    }
    
    void TickingClock::tick()
    {
        tickTime = MasterClock::getTime();
    }
}
