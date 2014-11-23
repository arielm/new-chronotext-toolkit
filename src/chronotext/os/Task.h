/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * ADDITIONAL CONTRIBUTORS: MICHAEL BOCCARA
 */

/*
 *  TODO:
 *
 * 1) TEST FURTHER:
 *    - NEW WAY TO RUN SYNCHRONOUS TASKS VIA performRun
 *    - IS NEW Task::post PROPERLY ACCESSIBLE FROM SUB-CLASSES?
 *
 * 2) TRY TO USE REPLACE cinder::sleep() BY:
 *    - boost::this_thread::sleep()
 *    - OR EVEN BETTER: A PURE C++11 SOLUTION
 *
 * 3) TRY TO USE std::mutex AND std::lock_guard INSTEAD OF boost::mutex AND boost::mutex::scoped_lock
 *
 * 4) SEE IF THE NEW C++11 thread_local CAN BE USED FOR THE FOLLOWING:
 *    - bool cancelRequired
 *    - TaskManager *manager
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
        virtual void sleep(float milliseconds);
        
    protected:
        bool started;
        bool ended;
        bool cancelRequired;
        bool synchronous;
        
        std::thread _thread;
        boost::mutex _mutex;
        
        bool start(bool forceSync);
        bool cancel();
        bool isCancelRequired();
        
        void performRun();
        void performDetach();
        
        template <typename F>
        bool post(F &&fn);
        
    private:
        friend class TaskManager;
        TaskManager *manager; // TODO: SHOULD BE A shared_ptr (PRE-CONDITION: NO (PUBLIC) WAY TO CREATE A NON-SHARED TaskManager, CF Cinder::Timeline)
    };
}

namespace chr = chronotext;
