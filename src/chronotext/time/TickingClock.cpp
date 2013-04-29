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
