/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/os/TaskManager.h"
#include "chronotext/Context.h"

using namespace std;

namespace chr
{
    atomic<bool> TaskManager::LOG_VERBOSE (false);
    atomic<int> TaskManager::MAX_CONCURRENT_THREADS (4); // XXX: RAW MECHANISM, AS FOR NOW
    
    Task* TaskManager::getTask(int taskId)
    {
        if (os::isThreadSafe())
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
        if (os::isThreadSafe())
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
        if (os::isThreadSafe())
        {
            auto element = tasks.find(taskId);
            
            if (element != tasks.end())
            {
                if (!startedTasks.count(taskId) && !postponedTasks.count(taskId))
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
                         * ALLOW TASKS TO "REQUIRE" POSTPONING?
                         * E.G. VIA SOME ENUM RETURNED BY Task::start()
                         */
                        
                        if ((MAX_CONCURRENT_THREADS > 0) && (startedTasks.size() >= MAX_CONCURRENT_THREADS))
                        {
                            postponedTasks.insert(taskId);
                            taskQueue.push(taskId);
                        }
                        else
                        {
                            startedTasks.insert(taskId);
                            task->start(false);
                        }
                    }
                    
                    return true;
                }
                
            }
        }
        
        return false;
    }
    
    bool TaskManager::cancelTask(int taskId)
    {
        if (os::isThreadSafe())
        {
            auto it = tasks.find(taskId);
            
            if (it != tasks.end())
            {
                auto task = it->second;
                
                if (startedTasks.count(taskId))
                {
                    return task->cancel();
                }
                else
                {
                    task->performShutdown();
                    
                    tasks.erase(it);
                    postponedTasks.erase(taskId);
                }
                
                return true;
            }
        }
        
        return false;
    }
    
    // ---
    
    /*
     * POSTED (I.E. INVOKED ON THE SKETCH-THREAD) FROM Task::performRun()
     */
    void TaskManager::endTask(int taskId)
    {
        assert(os::isThreadSafe());
        
        auto it = tasks.find(taskId);
        assert(it != tasks.end());
        
        it->second->performShutdown();
        
        tasks.erase(it);
        startedTasks.erase(taskId);
        
        nextTask();
    }
    
    bool TaskManager::nextTask()
    {
        assert(os::isThreadSafe());
        
        if (!taskQueue.empty())
        {
            auto taskId = taskQueue.front();
            
            auto it = tasks.find(taskId);
            assert(it != tasks.end());

            taskQueue.pop();
            postponedTasks.erase(taskId);
            
            startedTasks.insert(taskId);
            it->second->start(false);
            
            return true;
        }
        
        return false;
    }
}
