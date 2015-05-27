/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/os/Task.h"
#include "chronotext/os/TaskManager.h"
#include "chronotext/Context.h"

#include <chrono>

using namespace std;
using namespace ci;

namespace chr
{
    void Task::sleep(double seconds)
    {
        this_thread::sleep_for(chrono::duration<double, chrono::seconds::period>(seconds));
    }
    
    Task::~Task()
    {
        LOGI_IF(TaskManager::LOG_VERBOSE) << __PRETTY_FUNCTION__ << " | " << taskId << " | " << this << endl;
        
        detach(); // OTHERWISE: THE APPLICATION MAY CRASH WHEN SHUT-DOWN ABRUPTLY (REPRODUCIBLE ON OSX)
    }
    
    int Task::getId()
    {
        lock_guard<mutex> lock(_mutex); // SEE: Task::performInit()
        return taskId;
    }
    
    bool Task::isRunning()
    {
        lock_guard<mutex> lock(_mutex); // SEE: Task::start() AND Task::performRun()
        return (started && !ended);
    }
    
    bool Task::isAwaitingCancellation()
    {
        lock_guard<mutex> lock(_mutex); // SEE: Task::cancel()
        return cancelRequired;
    }
    
    // ---
    
    bool Task::post(function<void()> &&fn)
    {
        if (isRunning())
        {
            return os::post(forward<function<void()>>(fn), synchronous);
        }
        
        return false;
    }
    
    // ---
    
    /*
     * INVOKED ON THE SKETCH-THREAD
     *
     * OPTION 1: FROM TaskManager::addTask()
     * OPTION 2: FROM TaskManager::nextTask()
     */
    void Task::start(bool forceSync)
    {
        assert(initialized && !started);
        
        _mutex.lock(); // SEE: Task::isRunning()
        
        synchronous = forceSync;
        started = true;

        LOGI_IF(TaskManager::LOG_VERBOSE) << __PRETTY_FUNCTION__ << " | " << taskId << " | " << this << endl;

        if (synchronous)
        {
            _mutex.unlock(); // IDEM
            run();
            
            _mutex.lock(); // IDEM
            ended = true;
        }
        else
        {
            _thread = thread(&Task::performRun, this);
        }
        
        _mutex.unlock(); // IDEM
    }
    
    /*
     * INVOKED ON THE SKETCH-THREAD, FROM TaskManager::cancelTask()
     */
    bool Task::cancel()
    {
        assert(!synchronous && started);
        
        lock_guard<mutex> lock(_mutex); // REASON: Task::isAwaitingCancellation()
        
        if (!cancelRequired)
        {
            LOGI_IF(TaskManager::LOG_VERBOSE) << __PRETTY_FUNCTION__ << " | " << taskId << " | " << this << endl;
            
            cancelRequired = true;
            return true;
        }
        
        return false;
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
        assert(!initialized && !ended);
        
        lock_guard<mutex> lock(_mutex); // SEE: Task::getId()
        
        if (init())
        {
            LOGI_IF(TaskManager::LOG_VERBOSE) << __PRETTY_FUNCTION__ << " | " << taskId << " | " << this << endl;
            
            Task::manager = manager;
            Task::taskId = taskId;
            
            initialized = true;
            return true;
        }
        
        return false;
    }
    
    /*
     * INVOKED ON THE SKETCH-THREAD
     *
     * OPTION 1: FROM TaskManager::addTask()
     * OPTION 2: FROM TaskManager::cancelTask()
     * OPTION 3: FROM TaskManager::endTask()
     */
    void Task::performShutdown()
    {
        assert(initialized && (!started || ended));
        
        LOGI_IF(TaskManager::LOG_VERBOSE) << __PRETTY_FUNCTION__ << " | " << taskId << " | " << this << endl;
        
        shutdown();
        initialized = false;
        
        /*
         * TODO: TRY "JOINING" INSTEAD OF "DETACHING"
         */
        detach();
    }
    
    /*
     * INVOKED ON THE TASK-THREAD, FROM Task::start()
     */
    void Task::performRun()
    {
        assert(started && !ended);
        
        LOGI_IF(TaskManager::LOG_VERBOSE) << __PRETTY_FUNCTION__ << " [BEGIN] | " << taskId << " | " << this << endl;

        {
            /*
             * ThreadSetup IS MANDATORY ON OSX AND iOS (DUMMY ON ANDROID AND WINDOWS)
             *
             * PURPOSELY "BLOCK-SCOPED", IN ORDER TO "EXPIRE" RIGHT AFTER run()
             *
             * TODO: A SIMILAR SYSTEM IS NECESSARY ON ANDROID FOR ATTACHING/DETACHING THE THREAD TO/FROM JAVA
             */
            ThreadSetup forCocoa;
            
            run();
        }
        
        lock_guard<mutex> lock(_mutex); // SEE: Task::isRunning()
        
        ended = true;
        
        LOGI_IF(TaskManager::LOG_VERBOSE) << __PRETTY_FUNCTION__ << " [END] | " << taskId << " | " << this << endl;
        
        /*
         * IT IS NECESSARY TO WAIT FOR THE FUNCTIONS WHICH MAY HAVE BEEN POSTED DURING Task::run()
         *
         * TODO: CONSIDER USING A LAMBDA INSTEAD OF bind
         */
        os::post(bind(&TaskManager::endTask, manager, taskId), false);
        
        /*
         * TODO:
         *
         * INVESTIGATE IN WHICH "STATE" IS (AND/OR SHOULD BE) THE THREAD, UNTIL Task::performShutdown() IS INVOKED:
         *
         * - IS IT ENOUGH TO BE IN THE (SUPPOSEDLY) CURRENT "NON-DETACHED STATE"?
         * - OR WOULD IT BE PREFERRABLE TO BE IN A "NON-BUSY-WAIT STATE" UNTIL THEN?
         */
    }
}
