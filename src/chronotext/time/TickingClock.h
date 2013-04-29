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
