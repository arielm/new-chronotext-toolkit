/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/time/TimeBase.h"

#include "cinder/Timer.h"

namespace chr
{
    class DefaultTimeBase : public TimeBase
    {
    public:
        static std::shared_ptr<DefaultTimeBase> create()
        {
            return std::shared_ptr<DefaultTimeBase>(new DefaultTimeBase()); // XXX: std::maked_shared ONLY WORKS WITH PUBLIC CONSTRUCTORS
        }

        double getTime()
        {
            return timer.getSeconds();
        }
        
    protected:
        DefaultTimeBase()
        {
            timer.start();
        }

        ci::Timer timer;
    };
}
