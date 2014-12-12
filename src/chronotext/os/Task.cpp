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
    
    bool Task::canBeRemoved()
    {
        boost::mutex::scoped_lock lock(_mutex);
        return started && !cancelRequired;
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
    
    bool Task::post(function<void()> &&fn)
    {
        /*
         * ASSERTION: INVOKED ON THE TASK'S THREAD
         */
        
        if (manager && started)
        {
            return manager->post(forward<function<void()>>(fn), synchronous);
        }
        
        return false;
    }
    
    // ---
    
    bool Task::start()
    {
        boost::mutex::scoped_lock lock(_mutex);
        
        if (!synchronous && !started && !ended)
        {
            if (manager)
            {
                started = true; // TENTATIVELY...
                
                if (manager->post([=]{ _thread = thread(&Task::performRun, this); }, false)) // TODO: TEST
                {
                    LOGD_IF(VERBOSE) << __PRETTY_FUNCTION__ << " [SUCCESS] | " << taskId << " | " << this << endl;
                    return true;
                }
                else
                {
                    started = false;
                }
            }
        }
        
        LOGD_IF(VERBOSE) << __PRETTY_FUNCTION__ << " [FAILURE] | " << taskId << " | " << this << endl;
        return false;
    }
    
    bool Task::cancel()
    {
        boost::mutex::scoped_lock lock(_mutex);
        
        if (!synchronous && !cancelRequired && started)
        {
            LOGD_IF(VERBOSE) << __PRETTY_FUNCTION__ << " [SUCCESS] | " << taskId << " | " << this << endl;
            
            cancelRequired = true;
            return true;
        }
        
        LOGD_IF(VERBOSE) << __PRETTY_FUNCTION__ << " [FAILURE] | " << taskId << " | " << this << endl;
        return false;
    }
    
    void Task::detach()
    {
        if (!synchronous)
        {
            /*
             * ASSERTION: FOLLOWING IS SUPPOSED TO WORK NO MATTER ON WHICH THREAD INVOKED (TODO: VERIFY)
             */
            
            if (_thread.joinable())
            {
                _thread.detach();
            }
        }
    }
    
    bool Task::performInit(shared_ptr<TaskManager> manager, int taskId)
    {
        boost::mutex::scoped_lock lock(_mutex);
        
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
    
    void Task::performRun()
    {
        if (!synchronous)
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
            
            started = false;
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
     * OPTION 1: INVOKED ON ANY THREAD (TODO: VERIFY), FROM TaskManager::cancelTask()
     * OPTION 2: INVOKED ON THE IO-THREAD, FROM TaskManager::endTask()
     */
    
    void Task::performShutdown()
    {
        LOGD_IF(VERBOSE) << __PRETTY_FUNCTION__ << " | " << taskId << " | " << this << endl;
        
        shutdown();
        
        /*
         * TODO: TRY "JOINING" INSTEAD OF "DETACHING"
         */
        detach();
    }
}
