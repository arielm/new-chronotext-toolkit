/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * ADDITIONAL CONTRIBUTORS: MICHAEL BOCCARA
 */

/*
 *  TODO:
 *
 * 1) TRY TO USE boost::this_thread::sleep() INSTEAD OF cinder::sleep()
 *    REQUIREMENTS: MUST BE TESTED ON OSX / iOS / WINDOWS / ANDROID
 */

#pragma once

#include "cinder/Thread.h"

#include <boost/thread/mutex.hpp>

namespace chronotext
{
    class TaskManager;
    
    class Task
    {
    public:
        Task();
        virtual ~Task();
        
        virtual void run() = 0;
        void sleep(float milliseconds);
        
        friend class TaskManager;
        
    protected:
        bool started;
        bool ended;
        bool cancelRequired;
        bool synchronized;
        
        TaskManager *manager;
        
        std::thread _thread;
        boost::mutex _mutex;
        
        bool start(bool forceSync);
        bool cancel();
        bool isCancelRequired();
        
        void performRun();
        void performDetach();
    };
}

namespace chr = chronotext;
