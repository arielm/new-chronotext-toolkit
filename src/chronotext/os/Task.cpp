/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "Task.h"

#include "chronotext/Context.h"
#include "chronotext/utils/Utils.h"

#include <chrono>

using namespace std;
using namespace ci;

using namespace context;

namespace chr
{
    atomic<bool> Task::LOG_VERBOSE (false);
    
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
    cancelRequired(false),
    synchronous(false),
    taskId(0)
    {}
    
    Task::~Task()
    {
        LOGI_IF(LOG_VERBOSE) << __PRETTY_FUNCTION__ << " | " << state.taskId << " | " << this << endl;
        
        detach(); // OTHERWISE: THE APPLICATION MAY CRASH WHEN SHUT-DOWN ABRUPTLY
    }
    
    int Task::getId()
    {
        lock_guard<mutex> lock(_mutex);
        return state.taskId;
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
     * - ONLY ACCESSIBLE FROM TASKS
     * - INTENDED TO BE INVOKED ON THE TASK-THREAD
     */
    
    bool Task::post(function<void()> &&fn)
    {
        if (state.started && !state.ended)
        {
            return os::post(forward<function<void()>>(fn), state.synchronous);
        }
        
        return false;
    }
    
    // ---
    
    /*
     * INVOKED ON THE SKETCH-THREAD, FROM TaskManager::addTask()
     */
    
    void Task::start(bool forceSync)
    {
        if (state.initialized && !state.started)
        {
            lock_guard<mutex> lock(_mutex);
            
            LOGI_IF(LOG_VERBOSE) << __PRETTY_FUNCTION__ << " | " << state.taskId << " | " << this << endl;
            
            state.synchronous = forceSync;
            
            if (forceSync)
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
        if (!state.synchronous && state.started)
        {
            lock_guard<mutex> lock(_mutex);

            LOGI_IF(LOG_VERBOSE) << __PRETTY_FUNCTION__ << " | " << state.taskId << " | " << this << endl;
            
            state.cancelRequired = true;
        }
        else
        {
            assert(false);
        }
    }
    
    /*
     * ASSERTIONS:
     *
     * - SUPPOSED TO WORK NO MATTER ON WHICH THREAD INVOKED
     * - NO-HARM IF _thread WAS NEVER ACTUALLY USED
     */
    
    void Task::detach()
    {
        if (_thread.joinable())
        {
            _thread.detach();
        }
    }
    
    /*
     * INVOKED ON THE SKETCH-THREAD, FROM TaskManager::registerTask()
     */
    
    bool Task::performInit(shared_ptr<TaskManager> manager, int taskId)
    {
        if (!state.initialized)
        {
            lock_guard<mutex> lock(_mutex);

            if (init())
            {
                LOGI_IF(LOG_VERBOSE) << __PRETTY_FUNCTION__ << " | " << state.taskId << " | " << this << endl;
                
                Task::manager = manager;
                state.taskId = taskId;
                
                state.initialized = true;
                return true;
            }
            
            return false;
        }
        else
        {
            assert(false);
        }
    }
    
    /*
     * INTENDED TO BE INVOKED FROM Task::start(), ON THE TASK-THREAD
     */
    
    void Task::performRun()
    {
        if (state.started && !state.ended)
        {
            LOGI_IF(LOG_VERBOSE) << __PRETTY_FUNCTION__ << " [BEGIN] | " << state.taskId << " | " << this << endl;
            
            if (!isCancelRequired()) // TODO: DOUBLE-CHECK IF TEST MAKES SENSE
            {
                /*
                 * ThreadSetup IS MANDATORY ON OSX AND iOS (DUMMY ON ANDROID AND WINDOWS)
                 *
                 * TODO: A SIMILAR SYSTEM IS NECESSARY ON ANDROID FOR ATTACHING/DETACHING THE THREAD TO/FROM JAVA
                 */
                ThreadSetup forCocoa;
                
                run();
            }
            
            lock_guard<mutex> lock(_mutex); // TODO: FOLLOW-UP
            
            state.ended = true;

            LOGI_IF(LOG_VERBOSE) << __PRETTY_FUNCTION__ << " [END] | " << state.taskId << " | " << this << endl;

            /*
             * IT IS NECESSARY TO WAIT FOR THE FUNCTIONS WHICH MAY HAVE BEEN POSTED DURING Task::run()
             *
             * TODO: CONSIDER USING A LAMBDA INSTEAD OF bind
             */
            os::post(bind(&TaskManager::endTask, manager, state.taskId), false);
            
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
            LOGI_IF(LOG_VERBOSE) << __PRETTY_FUNCTION__ << " | " << state.taskId << " | " << this << endl;
            
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
