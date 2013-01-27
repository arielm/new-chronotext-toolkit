#include "chronotext/time/Clock.h"
#include "chronotext/time/MasterClock.h"
#include "chronotext/utils/Utils.h"

using namespace std;

namespace chronotext
{
    Clock::Clock()
    :
    timeBaseIsOwned(true),
    mst(0),
    rate(1),
    state(STOPPED)
    {
        timeBase = new DefaultTimeBase();
    }
    
    Clock::Clock(TimeBase *timeBase)
    :
    timeBase(timeBase),
    timeBaseIsOwned(false),
    mst(0),
    rate(1),
    state(STOPPED)
    {}
    
    Clock::Clock(MasterClockRef master)
    :
    timeBase(master->timeBase),
    timeBaseIsOwned(false),
    master(master),
    mst(0),
    rate(1),
    state(STOPPED)
    {
        master->add(this);
    }
    
    Clock::~Clock()
    {
        DLOG("Clock DELETED");
        
        if (timeBaseIsOwned)
        {
            delete timeBase;
        }
        
        if (master)
        {
            master->remove(this);
        }
    }
    
    void Clock::start()
    {
        tbst = timeBase->getTime();
        state = STARTED;
    }
    
    void Clock::restart()
    {
        mst = 0;
        start();
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
    
    void Clock::setTime(int now)
    {
        mst = now;
    }
    
    int Clock::getState()
    {
        return state;
    }
    
    void Clock::setRate(double factor)
    {
        rate = factor;
    }
}
