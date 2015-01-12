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
 *  TODO:
 *
 * 1) TEST AND DEVELOP FURTHER:
 *    - SEE "INNER" TODOS IN TaskManager AND Task
 *
 * 2) IMPLEMENT THREAD ATTACHMENT/DETACHMENT TO/FROM JAVA ON ANDROID
 *    - STUDY JNI'S AttachCurrentThread / DetachCurrentThread
 */

#pragma once

#include "cinder/Thread.h"

namespace chr
{
    class TaskManager;
    
    class Task
    {
    public:
        static void sleep(double seconds);

        /*
         * INVOKED ON THE SKETCH-THREAD, DURING REGISTRATION
         *
         * RETURNING false DISCARDS REGISTRATION
         */
        virtual bool init() { return true; }
        
        /*
         * INVOKED ON THE SKETCH-THREAD, AFTER EXECUTION
         */
        virtual void shutdown() {}
        
        /*
         * SYNCHRONOUS TASKS: INVOKED ON THE SKETCH-THREAD
         * ASYNCHRONOUS TASKS: INVOKED ON THE TASK-THREAD
         */
        virtual void run() = 0;
        
        /*
         * THE FOLLOWING 4 CAN BE INVOKED FROM ANY THREAD
         */
        int getId();
        bool isRunning();
        bool isAwaitingCancellation();
        
    protected:
        int taskId = 0;
        bool synchronous = false;
        
        virtual ~Task();

        /*
         * RETURNS FALSE IF THE FUNCTION CAN'T BE POSTED
         *
         * CAUSES:
         *
         * - SEE os::post() IN chronotext/Context.h
         * - NOT INVOKED WHILE RUNNING
         */
        bool post(std::function<void()> &&fn);
        
    private:
        friend class TaskManager;
        
        bool initialized = false;
        bool started = false;
        bool ended = false;
        bool cancelRequired = false;
        
        std::shared_ptr<TaskManager> manager;
        std::thread _thread;
        std::mutex _mutex;
        
        void start(bool forceSync = false);
        bool cancel();
        void detach();
        
        bool performInit(std::shared_ptr<TaskManager> manager, int taskId);
        void performShutdown();
        void performRun();
    };
}
