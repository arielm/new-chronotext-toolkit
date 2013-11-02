/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/time/Clock.h"

#include <list>

namespace chronotext
{
    class MasterClock : public Clock
    {
    protected:
        std::list<Clock*> slaves;

    public:
        MasterClock() : Clock() {}
        MasterClock(TimeBase *timeBase) : Clock(timeBase) {}
        
        virtual void start();
        virtual void stop();
        
        void add(Clock *slave);
        void remove(Clock *slave);
    };
}

namespace chr = chronotext;
