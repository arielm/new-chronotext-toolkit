/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/time/Clock.h"

namespace chronotext
{
    class FrameClock : public Clock
    {
    public:
        static std::shared_ptr<FrameClock> create() { return std::shared_ptr<FrameClock>(new FrameClock()); } // XXX: std::maked_shared ONLY WORKS WITH PUBLIC CONSTRUCTORS
        
        double getTime();
        void setTime(double now);
        
        void update();
        
    protected:
        bool shouldSample;
        double frameTime;
        
        FrameClock();
        FrameClock(std::shared_ptr<TimeBase> timeBase);
    };
}
