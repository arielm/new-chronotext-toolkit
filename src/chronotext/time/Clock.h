/*
 * INSPIRED BY javax.media.Clock
 * http://docs.oracle.com/javame/config/cdc/opt-pkgs/api/jsr927/javax/media/Clock.html
 */

#pragma once

#include "chronotext/time/DefaultTimeBase.h"

namespace chronotext
{
    class MasterClock;
    
    typedef std::shared_ptr<class Clock> ClockRef;
    typedef std::shared_ptr<class MasterClock> MasterClockRef;

    class Clock : public TimeBase
    {
        double mst;
        double rate;
        int state;
        double tbst;

        TimeBase *timeBase;
        bool timeBaseIsOwned;
        
        MasterClockRef master;
        
    public:
        enum
        {
            STOPPED,
            STARTED
        };

        Clock();
        Clock(TimeBase *timeBase);
        Clock(MasterClockRef master);
        
        ~Clock();
        
        void start();
        void stop();
        double getTime();
        void setTime(int now);
        int getState();
        void setRate(double factor);
    };
}

namespace chr = chronotext;
