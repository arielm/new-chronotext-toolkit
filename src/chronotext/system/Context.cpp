/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "Context.h"

#include "chronotext/Log.h"

using namespace std;
using namespace ci;

namespace chronotext
{
    namespace context
    {
        namespace intern
        {
            std::shared_ptr<system::Manager> systemManager;
            std::shared_ptr<memory::Manager> memoryManager;
            
            boost::asio::io_service *io = nullptr;
            
            // ---
            
            bool initialized = false;
            bool setup = false;
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
        
        void setup(boost::asio::io_service &io)
        {
            if (!intern::setup && init())
            {
                intern::io = &io;
                
                // ---
                
                intern::setup = true;
            }
        }
        
        void shutdown()
        {
            if (intern::setup)
            {
                intern::memoryManager.reset();
                intern::systemManager.reset();
                
                intern::io = nullptr;
                
                // ---
                
                intern::setup = false;
                intern::initialized = false;
            }
        }
        
        system::Manager* systemManager()
        {
            return intern::systemManager.get();
        }
        
        memory::Manager* memoryManager()
        {
            return intern::memoryManager.get();
        }
        
        boost::asio::io_service& io()
        {
            return *intern::io;
        }
    }
}
