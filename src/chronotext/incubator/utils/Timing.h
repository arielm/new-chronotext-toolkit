/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/utils/Utils.h"

#include "cinder/Timer.h"

namespace chr
{
    class Timing
    {
    public:
        Timing(const std::string &message = "")
        :
        started(false)
        {
            start(message);
        }
        
        ~Timing()
        {
            stop();
        }
        
        void start(const std::string &message = "")
        {
            this->message = message;
            
            timer.start();
            started = true;
        }
        
        void stop()
        {
            if (started)
            {
                double elapsed = timer.getSeconds();
                
                timer.stop();
                started = false;
                
                std::string prefix = message.empty() ? "" : (message + ": ");
                LOGI << prefix << utils::format::duration(elapsed, 3) << std::endl;
            }
        }
        
    protected:
        std::string message;
        bool started;
        ci::Timer timer;
    };
}
