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
    
    Task::State::State()
    :
    initialized(false),
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
        return state.initialized ? 0 : taskId;
    }
    
    bool Task::hasStarted()
    {
        boost::mutex::scoped_lock lock(_mutex);
        return state.started;
    }
    
    bool Task::hasEnded()
    {
        boost::mutex::scoped_lock lock(_mutex);
        return state.ended;
    }
    
    bool Task::isCancelRequired()
    {
        boost::mutex::scoped_lock lock(_mutex);
        return state.cancelRequired;
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
     * INVOKED ON THE IO-THREAD, FROM TaskManager::addTask()
     */
    
    void Task::start(bool forceSync)
    {
        if (!state.started)
        {
            LOGD_IF(VERBOSE) << __PRETTY_FUNCTION__ << " | " << taskId << " | " << this << endl;
            
            synchronous = forceSync;
            
            if (synchronous)
            {
                state.started = true;
                run();
                state.ended = true;
            }
            else
            {
                state.started = true;
                _thread = thread(&Task::performRun, this);
            }
        }
        else
        {
            assert(false);
        }
    }
    
    /*
     * INVOKED ON THE IO-THREAD, FROM TaskManager::cancelTask()
     */
    
    void Task::cancel()
    {
        if (!synchronous && state.started)
        {
            LOGD_IF(VERBOSE) << __PRETTY_FUNCTION__ << " | " << taskId << " | " << this << endl;
            
            boost::mutex::scoped_lock lock(_mutex);
            state.cancelRequired = true;
        }
        else
        {
            assert(false);
        }
    }
    
    /*
     * ASSERTION: SUPPOSED TO WORK NO MATTER ON WHICH THREAD INVOKED (TODO: VERIFY)
     */
    
    void Task::detach()
    {
        if (!synchronous)
        {
            if (_thread.joinable())
            {
                _thread.detach();
            }
        }
    }
    
    /*
     * INVOKED ON THE IO-THREAD, FROM TaskManager::registerTask()
     */
    
    bool Task::performInit(shared_ptr<TaskManager> manager, int taskId)
    {
        if (!state.initialized)
        {
            if (init())
            {
                LOGD_IF(VERBOSE) << __PRETTY_FUNCTION__ << " | " << taskId << " | " << this << endl;
                
                Task::manager = manager;
                Task::taskId = taskId;
                
                state.initialized = true;
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
        if (state.started && !state.ended)
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
            
            state.ended = true;
            
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
        else
        {
            assert(false);
        }
    }
    
    /*
     * INVOKED ON THE IO-THREAD
     *
     * OPTION 1: FROM TaskManager::cancelTask()
     * OPTION 2: FROM TaskManager::endTask()
     */
    
    void Task::performShutdown()
    {
        if (!state.started || state.ended)
        {
            LOGD_IF(VERBOSE) << __PRETTY_FUNCTION__ << " | " << taskId << " | " << this << endl;
            
            shutdown();
            
            /*
             * TODO: TRY "JOINING" INSTEAD OF "DETACHING"
             */
            detach();
        }
        else
        {
            assert(false);
        }
    }
}
