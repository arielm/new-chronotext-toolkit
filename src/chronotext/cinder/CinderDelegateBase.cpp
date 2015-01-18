/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/cinder/CinderDelegateBase.h"
#include "chronotext/Context.h"

using namespace std;
using namespace ci;

namespace chr
{
    void CinderDelegateBase::messageFromBridge(int what, const string &body)
    {
        sketch->sendMessage(Message(what, body));
    }
    
    // ---
    
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
    
    bool CinderDelegateBase::_init()
    {
        if (!intern::initialized)
        {
            intern::systemManager = make_shared<SystemManager>();
            intern::systemManager->setup(initInfo);
            
            intern::memoryManager = make_shared<MemoryManager>();
            intern::memoryManager->setup();
            
            DisplayHelper::setup(initInfo);
            FileHelper::setup(initInfo);
            
            // ---
            
            sketch = createSketch();
            sketchCreated(sketch);
            sketch->init();
            
            // ---
            
            intern::initialized = true;
        }
        
        return intern::initialized;
    }
    
    void CinderDelegateBase::_launch()
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
            
            sketch->launch();

            // ---
            
            intern::launched = true;
        }
    }
    
    void CinderDelegateBase::_setup()
    {
        if (!intern::setup && intern::launched)
        {
            LOGI_IF(true) << "WINDOW INFO: " << setupInfo.windowInfo << endl; // LOG: VERBOSE
            
            sketch->performSetup(setupInfo.windowInfo);
            
            // ---
            
            intern::setup = true;
        }
    }
    
    void CinderDelegateBase::_shutdown()
    {
        if (intern::setup)
        {
            sketch->shutdown();
            delete sketch;
            sketchDestroyed(sketch);
            sketch = nullptr;

            // ---
            
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
            FileHelper::shutdown();
            DisplayHelper::shutdown();
            
            intern::memoryManager->shutdown();
            intern::memoryManager.reset();
            
            intern::systemManager->shutdown();
            intern::systemManager.reset();
            
            // ---
            
            intern::initialized = false;
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
    
    // ---
    
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
