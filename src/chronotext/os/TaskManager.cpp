/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "TaskManager.h"

#include "chronotext/Context.h"

using namespace std;

namespace chr
{
    TaskManager::TaskManager()
    :
    taskCount(0)
    {}
    
    Task* TaskManager::getTask(int taskId)
    {
        boost::mutex::scoped_lock lock(_mutex);
        
        auto element = tasks.find(taskId);
        
        if (element != tasks.end())
        {
            return element->second.get();
        }
        
        return nullptr;
    }
    
    int TaskManager::registerTask(shared_ptr<Task> task)
    {
        boost::mutex::scoped_lock lock(_mutex);
        
        if (task->performInit(shared_from_this(), taskCount + 1))
        {
            tasks[++taskCount] = task;
            return taskCount;
        }
        
        return 0;
    }
    
    bool TaskManager::addTask(int taskId, bool forceSync)
    {
        boost::mutex::scoped_lock lock(_mutex);
        
        auto element = tasks.find(taskId);
        
        if (element != tasks.end())
        {
            auto task = element->second.get();
            
            if (forceSync)
            {
                /*
                 * TODO: ENSURE IT IS INVOKED ON THE IO-THREAD
                 */
                
                task->synchronous = true;
                task->started = true;
                
                task->run();
                
                task->started = false;
                task->ended = false;
                
                task->performShutdown();
                tasks.erase(element);
                
                return true;
            }
            else
            {
                /*
                 * TODO:
                 *
                 * START IF "CONCURRENT-THREAD-QUOTA" IS NOT EXCEEDED
                 * OTHERWISE: POSTPONE
                 */
                
                return task->start();
            }
        }
        
        return false;
    }
    
    bool TaskManager::cancelTask(int taskId)
    {
        boost::mutex::scoped_lock lock(_mutex);
        
        auto element = tasks.find(taskId);
        
        if (element != tasks.end())
        {
            auto task = element->second;
            
            if (task->canBeRemoved())
            {
                task->performShutdown();
                tasks.erase(element);
                
                return true;
            }
            else
            {
                return element->second->cancel();
            }
        }
        
        return false;
    }
    
    // ---
    
    bool TaskManager::post(function<void()> &&fn, bool forceSync)
    {
        if (forceSync)
        {
            /*
             * ASSERTION: INVOKED ON THE IO-THREAD
             */
            
            fn();
            return true;
        }
        
        if (true) // TODO: SHOULD BE FALSE IF IO-SERVICE IS NOT DEFINED OR IF THE CONTEXT IS BEING SHUT-DOWN
        {
            context::io_service().post(fn);
            return true;
        }
        
        return false;
    }
    
    /*
     * INVOKED ON THE IO-THREAD, FOLLOWING "POST" FROM Task::performRun()
     */
    
    void TaskManager::endTask(int taskId)
    {
        auto element = tasks.find(taskId);
        
        if (element != tasks.end())
        {
            element->second->performShutdown();
            tasks.erase(element);
        }
    }
}
