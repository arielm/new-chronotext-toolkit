/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/Context.h"

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
        bool init(const system::InitInfo &initInfo)
        {
            if (!intern::initialized)
            {
                intern::systemManager = make_shared<system::Manager>();
                intern::systemManager->setup(initInfo);
                
                intern::memoryManager = make_shared<memory::Manager>();
                intern::memoryManager->setup();
                
                // ---
                
                intern::initialized = true;
            }
            
            return intern::initialized;
        }
        
        void setup(const system::SetupInfo &setupInfo)
        {
            if (!intern::setup && intern::initialized)
            {
                /*
                 * TODO: MOVE TO TaskManager
                 */
                intern::io_service = setupInfo.io_service;
                intern::threadId = this_thread::get_id();
                
                intern::taskManager = TaskManager::create();
                
                // ---
                
                intern::setup = true;
            }
        }
        
        void shutdown()
        {
            if (intern::setup)
            {
                /*
                 * TODO:
                 *
                 * - HANDLE PROPERLY THE SHUTING-DOWN OF "UNDERGOING" TASKS
                 * - SEE RELATED TODOS IN CinderDelegate AND TaskManager
                 */
                intern::taskManager.reset();
                intern::io_service = nullptr;
                
                // ---
                
                intern::setup = false;
            }
            
            if (intern::initialized)
            {
                intern::memoryManager->shutdown();
                intern::memoryManager.reset();
                
                intern::systemManager->shutdown();
                intern::systemManager.reset();
                
                // ---
                
                intern::initialized = false;
            }
        }
    }
}

// ---

using namespace chr;

namespace context
{
    system::Manager& systemManager()
    {
        assert(intern::initialized);
        return *intern::systemManager.get();
    }
    
    memory::Manager& memoryManager()
    {
        assert(intern::initialized);
        return *intern::memoryManager.get();
    }
    
    TaskManager& taskManager()
    {
        assert(intern::setup);
        return *intern::taskManager.get();
    }
    
    // ---
    
    /*
     * TODO: MOVE TO TaskManager
     */
    
    namespace os
    {
        bool isThreadSafe()
        {
            if (intern::setup)
            {
                return intern::threadId == this_thread::get_id();
            }
            
            return false;
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
}
