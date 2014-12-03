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
    class TaskManager : public std::enable_shared_from_this<TaskManager>
    {
    public:
        static std::shared_ptr<TaskManager> create(boost::asio::io_service &io)
        {
            return std::shared_ptr<TaskManager>(new TaskManager(io)); // XXX: std::make_shared ONLY WORKS WITH PUBLIC CONSTRUCTORS
        }

        template <typename F>
        inline void post(const F &fn, bool forceSync = false)
        {
            if (forceSync)
            {
                fn();
            }
            else
            {
                io.post(fn);
            }
        }
        
        /*
         * RETURNS -1 IF THE TASK CAN'T BE ADDED
         *
         * CAUSE:
         *
         * - THE TASK HAS ALREADY BEEN ADDED (I.E. TASKS ARE NOT REUSABLE)
         */
        int addTask(std::shared_ptr<Task> task);
        
        /*
         * RETURNS FALSE IF THE TASK CAN'T BE STARTED
         *
         * CAUSES:
         *
         * - NO TASK IS REGISTERED WITH THIS ID
         * - THE TASK HAS ALREADY BEEN STARTED
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
         *
         * - CANCELLING IS IRRELEVANT (SYNCHRONOUS TASKS ONLY)
         * - NO TASK IS REGISTERED WITH THIS ID
         * - THE TASK HAS NOT YET BEEN STARTED
         * - THE TASK HAS ALREADY ENDED
         * - CANCELLATION IS ALREADY UNDERGOING
         */
        bool cancelTask(int taskId);
        
    protected:
        friend class Task;

        boost::asio::io_service &io;
        boost::mutex _mutex;
        
        int lastId;
        std::map<int, std::shared_ptr<Task>> tasks;
        
        TaskManager(boost::asio::io_service &io);

        void taskEnded(Task *task);
    };
}
