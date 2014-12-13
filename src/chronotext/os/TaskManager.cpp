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
    {
        threadId = this_thread::get_id();
    }
    
    Task* TaskManager::getTask(int taskId)
    {
        if (isThreadSafe())
        {
            auto element = tasks.find(taskId);
            
            if (element != tasks.end())
            {
                return element->second.get();
            }
        }
        
        return nullptr;
    }
    
    int TaskManager::registerTask(shared_ptr<Task> task)
    {
        if (isThreadSafe())
        {
            for (auto &element : tasks)
            {
                if (element.second == task)
                {
                    return 0;
                }
            }
            
            if (task->performInit(shared_from_this(), taskCount + 1))
            {
                tasks[++taskCount] = task;
                return taskCount;
            }
        }
        
        return 0;
    }
    
    bool TaskManager::addTask(int taskId, bool forceSync)
    {
        if (isThreadSafe())
        {
            auto element = tasks.find(taskId);
            
            if (element != tasks.end())
            {
                if (!startedTasks.count(taskId))
                {
                    auto task = element->second;
                    
                    if (forceSync)
                    {
                        task->start(true);
                        
                        task->performShutdown();
                        tasks.erase(element);
                    }
                    else
                    {
                        /*
                         * TODO:
                         *
                         * 1) START ONLY IF "CONCURRENT-THREAD-QUOTA" IS NOT EXCEEDED
                         *    OTHERWISE: POSTPONE...
                         *
                         * 2) MAYBE ALLOW TASKS TO REQUIRE "POSTPONING"?
                         *    E.G. VIA SOME ENUM RETURNED BY Task::performStart()
                         */
                        
                        startedTasks.insert(taskId);
                        task->start(false);
                    }
                    
                    return true;
                }
                
            }
        }
        
        return false;
    }
    
    bool TaskManager::cancelTask(int taskId)
    {
        if (isThreadSafe())
        {
            auto element = tasks.find(taskId);
            
            if (element != tasks.end())
            {
                auto task = element->second;
                
                if (startedTasks.count(taskId))
                {
                    element->second->cancel();
                }
                else
                {
                    task->performShutdown();
                    tasks.erase(element);
                }
                
                return true;
            }
        }
        
        return false;
    }
    
    // ---
    
    bool TaskManager::isThreadSafe()
    {
        return threadId == this_thread::get_id();
    }
    
    // ---
    
    bool TaskManager::post(function<void()> &&fn, bool forceSync)
    {
        if (forceSync)
        {
            if (isThreadSafe())
            {
                fn();
                return true;
            }
        }
        else if (true) // TODO: SHOULD BE FALSE IF THE CONTEXT IS BEING SHUT-DOWN
        {
            context::io_service().post(fn);
            return true;
        }
        
        return false;
    }
    
    /*
     * POSTED FROM Task::performRun()
     */
    
    void TaskManager::endTask(int taskId)
    {
        assert(isThreadSafe());
        
        auto element = tasks.find(taskId);
        
        if (element != tasks.end())
        {
            element->second->performShutdown();
            tasks.erase(element);
        }
    }
}
