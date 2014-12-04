/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/Log.h"

#include "cinder/Timer.h"

namespace chronotext
{
    class Timing
    {
        std::string msg;
        bool started;
        ci::Timer timer;
        
    public:
        Timing(const std::string &msg = "")
        :
        started(false)
        {
            start(msg);
        }
        
        ~Timing()
        {
            stop();
        }
        
        void start(const std::string &msg = "")
        {
            if (!started)
            {
                this->msg = msg;
                
                timer.start();
                started = true;
            }
        }
        
        void stop()
        {
            if (started)
            {
                double tmp = timer.getSeconds();
                started = false;
                
                if (!msg.empty())
                {
                    LOGI << msg << ": ";
                }
                
                LOGI << std::setprecision(15) << tmp << std::endl;
            }
        }
    };
}
