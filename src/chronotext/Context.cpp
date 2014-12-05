/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "Context.h"

namespace chr
{
    namespace CONTEXT
    {
        namespace intern
        {
            std::shared_ptr<system::Manager> systemManager;
            std::shared_ptr<memory::Manager> memoryManager;
            
            boost::asio::io_service *io_service = nullptr;
            std::shared_ptr<TaskManager> taskManager;
            
            // ---
            
            bool initialized = false;
        }
        
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
            if (!intern::initialized && init())
            {
                intern::io_service = &io_service;
                intern::taskManager = TaskManager::create();
            }
        }
        
        void shutdown()
        {
            if (intern::initialized)
            {
                intern::memoryManager.reset();
                intern::systemManager.reset();
                
                /*
                 * TODO:
                 *
                 * PROPERLY HANDLE THE SHUTING-DOWN OF "UNDERGOING" TASKS
                 * RELATED TODOS IN CinderDelegate AND TaskManager
                 */
                intern::taskManager.reset();
                intern::io_service = nullptr;
                
                // ---
                
                intern::initialized = false;
            }
        }
    }
}

namespace context
{
    chr::system::Manager* systemManager()
    {
        return chr::CONTEXT::intern::systemManager.get();
    }
    
    chr::memory::Manager* memoryManager()
    {
        return chr::CONTEXT::intern::memoryManager.get();
    }
    
    boost::asio::io_service& io_service()
    {
        return *chr::CONTEXT::intern::io_service;
    }
    
    chr::TaskManager* taskManager()
    {
        return chr::CONTEXT::intern::taskManager.get();
    }
}
