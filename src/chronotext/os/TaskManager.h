/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * ADDITIONAL CONTRIBUTORS: MICHAEL BOCCARA
 */

/*
 * TODO:
 *
 * 1) TEST FURTHER:
 *    - NEWLY ADDED LOCK-GUARD
 *    - NEW WAY TO START ASYNCHRONOUS TASKS VIA IO-SERVICE
 *
 * 2) CHECK WHY IT IS NECESSARY TO CALL Task::PerformDetach UPON TASK COMPLETION
 *    - E.G. INSTEAD OF RELYING ON thread::join()
 *    - NECESSARY: SIMULATING EXTREME CASES (E.G. "EMPTY" ASYNCHRONOUS TASK, ETC.)
 *
 * 3) TRY TO RELY ON NEW C++11 FEATURES LIKE std::future AND std::async
 *
 * 4) CREATE TEST AND SAMPLE PROJECTS
 *    - REQUIREMENTS: MUST WORK AT LEAST ON OSX, iOS AND ANDROID
 */

#pragma once

#include "chronotext/os/Task.h"

#include <boost/asio.hpp>

namespace chronotext
{
    class TaskManager
    {
    public:
        TaskManager(boost::asio::io_service &io);
        
        template <typename F>
        inline void post(F &&fn, bool forceSync = false)
        {
            if (forceSync)
            {
                fn();
            }
            
            io.post(std::forward<F>(fn));
        }
        
        /*
         * RETURN -1 IF THE TASK CAN'T BE ADDED
         *
         * CAUSE:
         *
         * THE TASK HAS ALREADY BEEN ADDED (NO MATTER IF IT'S ALREADY STARTED, OF EVEN IF IT HAS ENDED)
         * I.E. TASKS ARE NOT REUSABLE
         */
        int addTask(std::shared_ptr<Task> task);
        
        /*
         * RETURNS FALSE IF THE TASK CAN'T BE STARTED
         *
         * CAUSES:
         *
         * - NO TASK IS REGISTERED WITH THIS ID
         * - ASYNCHRONOUS TASKS ONLY: IO-SERVICE HAS NOT BEEN DEFINED
         */
        bool startTask(int taskId, bool forceSync = false);

        inline bool startTask(std::shared_ptr<Task> &&task, bool forceSync = false)
        {
            return startTask(addTask(std::forward<std::shared_ptr<Task>>(task)), forceSync);
        }

        /*
         * RETURNS FALSE IF THE TASK CAN'T BE HINTED FOR CANCELLATION
         *
         * CAUSES:
         * - NO TASK IS REGISTERED WITH THIS ID
         * - SYNCHRONOUS TASKS ONLY: CANCELLING IS IRRELEVANT
         */
        bool cancelTask(int taskId);
        
    protected:
        friend class Task;

        boost::asio::io_service &io;
        boost::mutex _mutex;
        
        int lastId;
        std::map<int, std::shared_ptr<Task>> tasks;
        
        void taskEnded(Task *task);
    };
}
