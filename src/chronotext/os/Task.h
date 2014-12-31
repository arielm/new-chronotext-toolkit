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
        struct State
        {
            bool initialized;
            bool started;
            bool ended;
            bool cancelRequired;
            
            bool synchronous;
            int taskId;
            
            State();
        };

        static void sleep(double seconds);

        virtual bool init() { return true; }
        virtual void shutdown() {}
        virtual void run() = 0;
        
        /*
         * THE FOLLOWING 4 ARE THREAD-SAFE
         */
        int getId();
        bool hasStarted();
        bool hasEnded();
        bool isCancelRequired();
        
    protected:
        /*
         * USING A STRUCTURE AS A MEAN TO ENSURE PROPER INITIALIZATION
         *
         * NECESSARY BECAUSE WE DON'T WANT TO IMPOSE BASE-CLASS
         * CONSTRUCTOR-INVOCATION TO DERIVED CLASSES
         */
        State state;
        
        std::shared_ptr<TaskManager> manager;
        std::thread _thread;
        std::mutex _mutex;
        
        virtual ~Task();
        
        bool post(std::function<void()> &&fn);
        
    private:
        friend class TaskManager;
        
        void start(bool forceSync = false);
        void cancel();
        void detach();
        
        bool performInit(std::shared_ptr<TaskManager> manager, int taskId);
        void performShutdown();
        void performRun();
    };
}
