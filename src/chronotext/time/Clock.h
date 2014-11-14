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

#pragma once

#include "chronotext/time/DefaultTimeBase.h"

namespace chronotext
{
    class Clock : public TimeBase
    {
    public:
        enum
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
        virtual int getState();
        virtual void setRate(double factor);
        virtual void restart();
        
    protected:
        double mst;
        double rate;
        double tbst;
        int state;
        
        std::shared_ptr<TimeBase> timeBase;
    };
}
