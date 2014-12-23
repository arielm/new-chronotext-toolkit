/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "Task.h"

#include "chronotext/Context.h"
#include "chronotext/os/TaskManager.h"
#include "chronotext/utils/Utils.h"

#include <chrono>

using namespace std;
using namespace ci;

namespace chr
{
    bool Task::VERBOSE = false;
    
    void Task::sleep(double seconds)
    {
#if defined(CINDER_ANDROID)
        boost::this_thread::sleep_for(boost::chrono::duration<double, boost::chrono::seconds::period>(seconds)); // XXX: A CURRENT LIMITATION OF SAFETY-DANK'S ANDROID PORT
#else
        this_thread::sleep_for(chrono::duration<double, chrono::seconds::period>(seconds));
#endif
    }

    Task::State::State()
    :
    initialized(false),
    started(false),
    ended(false),
    cancelRequired(false)
    {}
    
    Task::~Task()
    {
        LOGI_IF(VERBOSE) << __PRETTY_FUNCTION__ << " | " << taskId << " | " << this << endl;
        
        detach(); // OTHERWISE: THE APPLICATION MAY CRASH WHEN ABRUPTLY SHUT-DOWN
    }
    
    int Task::getId() const
    {
        return state.initialized ? 0 : taskId;
    }
    
    bool Task::hasStarted()
    {
        lock_guard<mutex> lock(_mutex);
        return state.started;
    }
    
    bool Task::hasEnded()
    {
        lock_guard<mutex> lock(_mutex);
        return state.ended;
    }
    
    bool Task::isCancelRequired()
    {
        lock_guard<mutex> lock(_mutex);
        return state.cancelRequired;
    }
    
    // ---
    
    /*
     * ASSERTION: INVOKED ON THE TASK-THREAD
     */
    
    inline bool Task::post(function<void()> &&fn)
    {
        return context::post(forward<function<void()>>(fn), synchronous);
    }
    
    // ---
    
    /*
     * INVOKED ON THE SKETCH-THREAD, FROM TaskManager::addTask()
     */
    
    void Task::start(bool forceSync)
    {
        if (!state.started)
        {
            LOGI_IF(VERBOSE) << __PRETTY_FUNCTION__ << " | " << taskId << " | " << this << endl;
            
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
     * INVOKED ON THE SKETCH-THREAD, FROM TaskManager::cancelTask()
     */
    
    void Task::cancel()
    {
        lock_guard<mutex> lock(_mutex);

        if (!synchronous && state.started)
        {
            LOGI_IF(VERBOSE) << __PRETTY_FUNCTION__ << " | " << taskId << " | " << this << endl;
            
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
     * INVOKED ON THE SKETCH-THREAD, FROM TaskManager::registerTask()
     */
    
    bool Task::performInit(shared_ptr<TaskManager> manager, int taskId)
    {
        if (!state.initialized)
        {
            if (init())
            {
                LOGI_IF(VERBOSE) << __PRETTY_FUNCTION__ << " | " << taskId << " | " << this << endl;
                
                Task::manager = manager;
                Task::taskId = taskId;
                
                state.initialized = true;
                return true;
            }
        }
        
        return false;
    }
    
    /*
     * ASSERTION: INVOKED ON THE TASK-THREAD
     */
    
    void Task::performRun()
    {
        if (state.started && !state.ended)
        {
            LOGI_IF(VERBOSE) << __PRETTY_FUNCTION__ << " [BEGIN] | " << taskId << " | " << this << endl;
            
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
             * IT IS NECESSARY TO WAIT FOR THE FUNCTIONS WHICH MAY HAVE BEEN POSTED BY DURING Task::run()
             *
             * TODO: CONSIDER USING LAMBDA INSTEAD OF bind
             */
            context::post(bind(&TaskManager::endTask, manager, taskId), false);
            
            LOGI_IF(VERBOSE) << __PRETTY_FUNCTION__ << " [END] | " << taskId << " | " << this << endl;
            
            /*
             * TODO:
             *
             * INVESTIGATE IN WHICH "STATE" IS THE THREAD UNTIL Task::performShutdown() IS INVOKED
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
     * INVOKED ON THE SKETCH-THREAD
     *
     * OPTION 1: FROM TaskManager::cancelTask()
     * OPTION 2: FROM TaskManager::endTask()
     */
    
    void Task::performShutdown()
    {
        if (!state.started || state.ended)
        {
            LOGI_IF(VERBOSE) << __PRETTY_FUNCTION__ << " | " << taskId << " | " << this << endl;
            
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
