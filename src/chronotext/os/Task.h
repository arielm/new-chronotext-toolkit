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
        static bool VERBOSE;
        
        struct State
        {
            bool initialized;
            bool started;
            bool ended;
            bool cancelRequired;
            
            State();
        };
        
        virtual bool init() { return true; }
        virtual void shutdown() {}
        virtual void run() = 0;
        
        int getId() const;
        
        bool hasStarted();
        bool hasEnded();
        bool isCancelRequired();
        
        static void sleep(double seconds);
        
    protected:
        State state;
        
        std::shared_ptr<TaskManager> manager;
        int taskId;
        bool synchronous;
        
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
