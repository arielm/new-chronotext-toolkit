/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "TaskManager.h"

using namespace std;

namespace chronotext
{
    TaskManager::TaskManager()
    :
    lastId(0),
    io(nullptr)
    {}
    
    void TaskManager::setIOService(boost::asio::io_service &io)
    {
        this->io = &io;
    }
    
    int TaskManager::addTask(shared_ptr<Task> task)
    {
        if (!task->manager)
        {
            task->manager = this;
            tasks[++lastId] = task;
            
            return lastId;
        }
        else
        {
            return 0;
        }
    }
    
    bool TaskManager::startTask(shared_ptr<Task> task, bool forceSync)
    {
        return startTask(addTask(task), forceSync);
    }
    
    bool TaskManager::startTask(int taskId, bool forceSync)
    {
        auto entry = tasks.find(taskId);
        
        if (entry != tasks.end())
        {
            return entry->second->start(forceSync);
        }
        
        return false;
    }
    
    bool TaskManager::cancelTask(int taskId)
    {
        auto entry = tasks.find(taskId);
        
        if (entry != tasks.end())
        {
            return entry->second->cancel();
        }
        
        return false;
    }
    
    void TaskManager::taskEnded(Task *task)
    {
        for (auto entry : tasks)
        {
            if (entry.second.get() == task)
            {
                task->performDetach();
                tasks.erase(entry.first);
                return;
            }
        }
    }
}
