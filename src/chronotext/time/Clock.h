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

#include "chronotext/Exception.h"
#include "chronotext/time/DefaultTimeBase.h"

#include <memory>

namespace chronotext
{
    class Clock : public TimeBase, public std::enable_shared_from_this<Clock>
    {
    public:
        enum
        {
            STOPPED,
            STARTED
        };

        Clock();
        Clock(TimeBase *timeBase);
        Clock(std::shared_ptr<Clock> master);
        
        virtual ~Clock();
        
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
        
        TimeBase *timeBase;
        bool timeBaseIsOwned;
        
        std::shared_ptr<Clock> master;
    };
}
