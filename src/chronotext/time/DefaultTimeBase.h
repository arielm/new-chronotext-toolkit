#pragma once

#include "chronotext/time/TimeBase.h"

#include "cinder/Timer.h"

namespace chronotext
{
    class DefaultTimeBase : public TimeBase
    {
        ci::Timer timer;
        
    public:
        DefaultTimeBase()
        {
            timer.start();
        }
        
        double getTime()
        {
            return timer.getSeconds();
        }
    };
}

namespace chr = chronotext;
