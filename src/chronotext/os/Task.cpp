/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "Task.h"

#include "chronotext/os/TaskManager.h"

#include "chronotext/utils/Utils.h"

using namespace std;
using namespace ci;

namespace chr
{
    const bool Task::VERBOSE = true;
    
    Task::Task()
    :
    taskId(0),
    synchronous(false),
    started(false),
    ended(false),
    cancelRequired(false)
    {}
    
    Task::~Task()
    {
        LOGD_IF(VERBOSE) << __PRETTY_FUNCTION__ << " | " << taskId << " | " << this << endl;
        
        detach(); // OTHERWISE: OSX APPLICATION MAY CRASH, E.G. WHEN SHUT-DOWN VIA WINDOW-CLOSE-BUTTON
    }
    
    int Task::getId() const
    {
        return taskId;
    }
    
    bool Task::hasStarted()
    {
        boost::mutex::scoped_lock lock(_mutex);
        return started;
    }
    
    bool Task::hasEnded()
    {
        boost::mutex::scoped_lock lock(_mutex);
        return ended;
    }
    
    bool Task::isCancelRequired()
    {
        boost::mutex::scoped_lock lock(_mutex);
        return cancelRequired;
    }
    
    void Task::sleep(float milliseconds)
    {
        ci::sleep(milliseconds);
    }
    
    // ---
    
    /*
     * ASSERTION: INVOKED ON THE TASK'S THREAD
     */
    
    bool Task::post(function<void()> &&fn)
    {
        return manager->post(forward<function<void()>>(fn), synchronous);
    }
    
    // ---
    
    /*
     * POSTED FROM TaskManager::addTask()
     */
    
    void Task::start()
    {
        if (!synchronous && !started)
        {
            LOGD_IF(VERBOSE) << __PRETTY_FUNCTION__ << " | " << taskId << " | " << this << endl;
            
            started = true;
            _thread = thread(&Task::performRun, this);
        }
    }
    
    /*
     * INVOKED ON THE IO-THREAD, FROM TaskManager::cancelTask()
     */
    
    bool Task::cancel()
    {
        boost::mutex::scoped_lock lock(_mutex); // TODO: DOUBLE-CHECK IF REALLY NECESSARY
        
        if (!synchronous && !cancelRequired && started)
        {
            LOGD_IF(VERBOSE) << __PRETTY_FUNCTION__ << " | " << taskId << " | " << this << endl;
            
            cancelRequired = true;
            return true;
        }
        
        return false;
    }
    
    void Task::detach()
    {
        if (!synchronous)
        {
            /*
             * ASSERTION: THE FOLLOWING IS SUPPOSED TO WORK NO MATTER ON WHICH THREAD INVOKED (TODO: VERIFY)
             */
            
            if (_thread.joinable())
            {
                _thread.detach();
            }
        }
    }
    
    bool Task::performInit(shared_ptr<TaskManager> manager, int taskId)
    {
        boost::mutex::scoped_lock lock(_mutex); // TODO: DOUBLE-CHECK IF REALLY NECESSARY
        
        if (!Task::manager)
        {
            if (init())
            {
                Task::manager = manager;
                Task::taskId = taskId;
                
                return true;
            }
        }
        
        return false;
    }
    
    /*
     * ASSERTION: INVOKED ON THE TASK'S THREAD
     */
    
    void Task::performRun()
    {
        if (started && !ended)
        {
            LOGD_IF(VERBOSE) << __PRETTY_FUNCTION__ << " [BEGIN] | " << taskId << " | " << this << endl;
            
            if (!isCancelRequired())
            {
                /*
                 * ThreadSetup IS MANDATORY ON OSX AND iOS (DUMMY ON ANDROID AND WINDOWS)
                 *
                 * TODO: A SIMILAR SYSTEM IS NECESSARY ON ANDROID FOR ATTACHING/DETACHING THE THREAD TO/FROM JAVA
                 */
                ThreadSetup forCocoa;
                
                run();
            }
            
            ended = true;
            
            /*
             * NECESSARY IN ORDER TO WAIT FOR THE LAMBDAS WHICH
             * MAY HAVE BEEN POSTED BY DURING Task::run()
             */
            manager->post([=]{ manager->endTask(taskId); });
            
            LOGD_IF(VERBOSE) << __PRETTY_FUNCTION__ << " [END] | " << taskId << " | " << this << endl;
            
            /*
             * TODO:
             *
             * INVESTIGATE IN WHICH "STATE" IS THE THREAD
             * UNTIL Task::performShutdown() IS INVOKED
             *
             * SHOULD WE BE IN A "WAIT STATE" UNTIL THEN?
             */
        }
    }
    
    /*
     * OPTION 1: INVOKED ON ANY THREAD, FROM TaskManager::cancelTask()
     * OPTION 2: POSTED FROM FROM TaskManager::endTask()
     */
    
    void Task::performShutdown()
    {
        if (!started || ended)
        {
            LOGD_IF(VERBOSE) << __PRETTY_FUNCTION__ << " | " << taskId << " | " << this << endl;
            
            shutdown();
            
            /*
             * TODO: TRY "JOINING" INSTEAD OF "DETACHING"
             */
            detach();
        }
    }
}
