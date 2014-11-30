/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "TaskManager.h"

using namespace std;

namespace chronotext
{
    TaskManager::TaskManager(boost::asio::io_service &io)
    :
    io(io),
    lastId(-1)
    {}
    
    int TaskManager::addTask(shared_ptr<Task> task)
    {
        boost::mutex::scoped_lock lock(_mutex);
        
        if (!task->manager)
        {
            task->manager = this;
            tasks[++lastId] = task;
            
            return lastId;
        }
        
        return -1;
    }
    
    bool TaskManager::startTask(int taskId, bool forceSync)
    {
        boost::mutex::scoped_lock lock(_mutex);
        
        auto element = tasks.find(taskId);
        
        if (element != tasks.end())
        {
            if (forceSync)
            {
                return element->second->start(true);
            }

            post([=]{ element->second->start(false); }, false); // TODO: TEST
            return true;
        }
        
        return false;
    }
    
    bool TaskManager::cancelTask(int taskId)
    {
        boost::mutex::scoped_lock lock(_mutex);
        
        auto element = tasks.find(taskId);
        
        if (element != tasks.end())
        {
            return element->second->cancel(); // NO-OP FOR SYNCHRONOUS TASKS
        }
        
        return false;
    }
    
    void TaskManager::taskEnded(Task *task)
    {
        for (auto &element : tasks)
        {
            if (task == element.second.get())
            {
                task->performDetach(); // NO-OP FOR SYNCHRONOUS TASKS
                tasks.erase(element.first);
                return;
            }
        }
    }
}
