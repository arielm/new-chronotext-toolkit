/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * ADDITIONAL CONTRIBUTORS: MICHAEL BOCCARA
 */

/*
 * TODO:
 *
 * 1) TEST FORCED-SYNCHRONIZATION FURTHER
 *
 * 2) CHECK NECESSITY TO CALL Task::PerformDetach UPON TASK COMPLETION
 */

#pragma once

#include "chronotext/os/Task.h"

#include <boost/asio.hpp>

namespace chronotext
{
    class TaskManager
    {
    public:
        TaskManager();
        
        template <typename F> bool post(const F &fn, bool forceSync = false)
        {
            if (forceSync)
            {
                fn();
                return true;
            }
            else if (io)
            {
                io->post(fn);
                return true;
            }
            
            return false;
        }
        
        void setIOService(boost::asio::io_service &io);
        
        int addTask(std::shared_ptr<Task> task);
        bool startTask(std::shared_ptr<Task> task, bool forceSync = false);
        bool startTask(int taskId, bool forceSync = false);
        bool cancelTask(int taskId);
        
        friend class Task;
        
    protected:
        int lastId;
        boost::asio::io_service *io;
        std::map<int, std::shared_ptr<Task>> tasks;
        
        void taskEnded(Task *task);
    };
}
