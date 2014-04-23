/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/time/Clock.h"

#include <set>
#include <memory>

namespace chronotext
{
    class MasterClock : public Clock, public std::enable_shared_from_this<MasterClock>
    {
    public:
        MasterClock();
        MasterClock(TimeBase *timeBase);

        friend class Clock;
        
    protected:
        std::set<Clock*> slaves;
        
        void add(Clock *slave);
        void remove(Clock *slave);
    };
}

namespace chr = chronotext;
