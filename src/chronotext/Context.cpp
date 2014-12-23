/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "Context.h"

using namespace std;

namespace chr
{
    namespace intern
    {
        shared_ptr<system::Manager> systemManager;
        shared_ptr<memory::Manager> memoryManager;
        shared_ptr<TaskManager> taskManager;
        
        boost::asio::io_service *io_service = nullptr;
        thread::id threadId;
        
        // ---
        
        bool initialized = false;
        bool setup = false;
    }
    
    namespace CONTEXT
    {
        bool init()
        {
            if (!intern::initialized)
            {
                intern::systemManager = make_shared<system::Manager>();
                intern::memoryManager = make_shared<memory::Manager>();
                
                // ---

                intern::initialized = true;
            }
            
            return intern::initialized;
        }
        
        void setup(boost::asio::io_service &io_service)
        {
            if (!intern::setup && init())
            {
                intern::io_service = &io_service;
                intern::threadId = this_thread::get_id();

                intern::taskManager = TaskManager::create();
                
                // ---
                
                intern::setup = true;
            }
        }
        
        void shutdown()
        {
            if (intern::initialized && intern::setup)
            {
                intern::memoryManager.reset();
                intern::systemManager.reset();
                
                /*
                 * TODO: HANDLE PROPERLY THE SHUTING-DOWN OF "UNDERGOING" TASKS
                 */
                intern::taskManager.reset();
                intern::io_service = nullptr;
                
                // ---
                
                intern::initialized = false;
                intern::setup = false;
            }
        }
    }
}

// ---

using namespace chr;

namespace context
{
    system::Manager* systemManager()
    {
        return intern::systemManager.get();
    }
    
    memory::Manager* memoryManager()
    {
        return intern::memoryManager.get();
    }
    
    TaskManager* taskManager()
    {
        return intern::taskManager.get();
    }
    
    // ---
    
    bool isThreadSafe()
    {
        return intern::threadId == this_thread::get_id();
    }
    
    bool post(function<void()> &&fn, bool forceSync)
    {
        if (forceSync)
        {
            if (isThreadSafe())
            {
                fn();
                return true;
            }
        }
        else if (intern::io_service)
        {
            intern::io_service->post(forward<function<void()>>(fn));
            return true;
        }
        
        return false;
    }
}
