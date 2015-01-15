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
        shared_ptr<SystemManager> systemManager;
        shared_ptr<MemoryManager> memoryManager;
        shared_ptr<TaskManager> taskManager;
        
        /*
         * TODO: MOVE TO TaskManager
         */
        boost::asio::io_service *io_service = nullptr;
        thread::id threadId;
        
        // ---
        
        bool initialized = false;
        bool launched = false;
        bool setup = false;
    }
    
    namespace INTERN
    {
        bool init(const system::InitInfo &initInfo)
        {
            if (!intern::initialized)
            {
                intern::systemManager = make_shared<SystemManager>();
                intern::systemManager->setup(initInfo);
                
                intern::memoryManager = make_shared<MemoryManager>();
                intern::memoryManager->setup();
                
                // ---
                
                intern::initialized = true;
            }
            
            return intern::initialized;
        }
        
        void launch(const system::LaunchInfo &launchInfo)
        {
            if (!intern::launched && intern::initialized)
            {
                /*
                 * TODO: MOVE TO TaskManager
                 */
                intern::io_service = launchInfo.io_service;
                intern::threadId = this_thread::get_id();
                
                intern::taskManager = TaskManager::create();
                
                // ---
                
                intern::launched = true;
            }
        }
        
        void setup(const system::SetupInfo &setupInfo)
        {
            if (!intern::setup && intern::launched)
            {
                LOGI_IF(true) << "WINDOW INFO: " << setupInfo.windowInfo << endl; // LOG: VERBOSE
                
                intern::setup = true;
            }
        }
        
        void shutdown()
        {
            if (intern::setup)
            {
                intern::setup = false;
            }
            
            if (intern::launched)
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
                
                intern::launched = false;
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
    
    // ---
    
    SystemManager& systemManager()
    {
        return checkedReference(intern::systemManager.get());
    }
    
    MemoryManager& memoryManager()
    {
        return checkedReference(intern::memoryManager.get());
    }
    
    TaskManager& taskManager()
    {
        return checkedReference(intern::taskManager.get());
    }
    
    /*
     * TODO: MOVE TO TaskManager
     */
    
    namespace os
    {
        bool isThreadSafe()
        {
            if (intern::launched)
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
