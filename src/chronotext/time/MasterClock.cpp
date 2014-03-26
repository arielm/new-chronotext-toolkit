/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/time/MasterClock.h"

using namespace std;

namespace chronotext
{
    MasterClock::MasterClock()
    :
    Clock()
    {}
    
    MasterClock::MasterClock(TimeBase *timeBase)
    :
    Clock(timeBase)
    {}

    void MasterClock::start()
    {
        Clock::start();
        
        for (auto &slave : slaves)
        {
            slave->start();
        }
    }
    
    void MasterClock::stop()
    {
        Clock::stop();
        
        for (auto &slave : slaves)
        {
            slave->stop();
        }
    }
    
    void MasterClock::add(Clock *slave)
    {
        slaves.push_back(slave);
    }
    
    void MasterClock::remove(Clock *slave)
    {
        slaves.remove(slave);
    }
}
