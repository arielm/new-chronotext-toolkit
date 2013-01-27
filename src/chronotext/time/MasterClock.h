#pragma once

#include "chronotext/time/Clock.h"

#include <list>

namespace chronotext
{
    class MasterClock : public Clock
    {
        std::list<Clock*> slaves;

    public:
        MasterClock() : Clock() {}
        MasterClock(TimeBase *timeBase) : Clock(timeBase) {}
        
        void start();
        void stop();
        
        void add(Clock *slave);
        void remove(Clock *slave);
    };
}

namespace chr = chronotext;
