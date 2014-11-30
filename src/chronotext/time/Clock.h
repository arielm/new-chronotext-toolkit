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
 * PROBLEM: CREATING A Clock FROM A TimeBase WHICH IS NOT ENCLOSED IN A shared_ptr WILL CAUSE A CRASH
 *
 * SOLUTION: SIMILAR TO WHAT IS DONE IN cinder::Timeline
 *
 *
 * TODO:
 *
 * 1) IMPLEMENT SOLUTION:
 *    CLASSES EXTENDING TimeBase SHOULD HAVE A PROTECTED CONSTRUCTOR
 *    AND A STATIC METHOD FOR CREATING INSTANCES ENCLOSED IN A shared_ptr
 *
 * 2) ULTIMATELY MERGE WITH:
 *    https://github.com/arielm/new-chronotext-toolkit/commit/420ab71a823dc7e1e73a5d1d4f8a1d820c310d06
 */

#pragma once

#include "chronotext/Exception.h"
#include "chronotext/time/DefaultTimeBase.h"

namespace chronotext
{
    class Clock : public TimeBase
    {
    public:
        enum State
        {
            STOPPED,
            STARTED
        };

        Clock();
        Clock(std::shared_ptr<TimeBase> timeBase);
        
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
    };
}
