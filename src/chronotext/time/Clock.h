/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * INSPIRED BY javax.media.Clock
 * http://docs.oracle.com/javame/config/cdc/opt-pkgs/api/jsr927/javax/media/Clock.html
 */

/*
 * PROBLEM: CREATING A Clock FROM A TimeBase WHICH IS NOT ENCLOSED IN A shared_ptr WOULD CAUSE A CRASH
 *
 * SOLUTION: ENFORCING CREATION AS shared_ptr (SIMILAR TO WHAT IS DONE IN cinder::Timeline)
 */

#pragma once

#include "chronotext/Exception.h"
#include "chronotext/time/DefaultTimeBase.h"

namespace chronotext
{
    class Clock : public TimeBase
    {
    public:
        static std::shared_ptr<Clock> create() { return std::shared_ptr<Clock>(new Clock()); } // XXX: std::maked_shared ONLY WORKS WITH PUBLIC CONSTRUCTORS

        enum State
        {
            STOPPED,
            STARTED
        };

        virtual void start();
        virtual void stop();
        
        virtual double getTime();
        virtual void setTime(double now);
        
        virtual double getRate();
        virtual void setRate(double factor);
        
        virtual State getState();
        
        virtual void restart();
        virtual void reset();
        
    protected:
        double mst;
        double rate;
        double tbst;
        State state;
        
        std::shared_ptr<TimeBase> timeBase;
        
        Clock();
        Clock(std::shared_ptr<TimeBase> timeBase);
    };
}
