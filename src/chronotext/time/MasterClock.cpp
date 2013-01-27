#include "chronotext/time/MasterClock.h"

using namespace std;

namespace chronotext
{
    void MasterClock::start()
    {
        Clock::start();
        
        for (list<Clock*>::const_iterator it = slaves.begin(); it != slaves.end(); ++it)
        {
            (*it)->start();
        }
    }
    
    void MasterClock::stop()
    {
        Clock::stop();
        
        for (list<Clock*>::const_iterator it = slaves.begin(); it != slaves.end(); ++it)
        {
            (*it)->stop();
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
