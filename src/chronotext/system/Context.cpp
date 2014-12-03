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
    Context::Context()
    {
        LOGI << __PRETTY_FUNCTION__ << endl;
        
        context::init(); // TODO: HANDLE FAILURE
    }
    
    Context::~Context()
    {
        LOGI << __PRETTY_FUNCTION__ << endl;
        
        context::uninit();
    }
}

namespace chronotext
{
    namespace context
    {
        namespace intern
        {
            bool initialized = false;
            
            // ---
            
            std::shared_ptr<system::Manager> systemManager;
        }
        
        bool init()
        {
            if (!intern::initialized)
            {
                intern::systemManager = make_shared<system::Manager>();
                
                if (intern::systemManager->init())
                {
                    intern::initialized = true;
                }
            }
            
            return intern::initialized;
        }
        
        void uninit()
        {
            if (intern::initialized)
            {
                intern::systemManager->uninit();
                intern::systemManager.reset();
                
                // ---
                
                intern::initialized = false;
            }
        }
        
        system::Manager* systemManager()
        {
            return intern::systemManager.get();
        }
    }
}
