/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/time/MasterClock.h"

namespace chronotext
{
    class TickingClock : public MasterClock
    {
        double tickTime;
        
    public:
        TickingClock() : MasterClock(), tickTime(0) {}
        TickingClock(TimeBase *timeBase) : MasterClock(timeBase), tickTime(0) {}
        
        double getTime();
        void setTime(int now);
        
        void tick();
    };
}

namespace chr = chronotext;
