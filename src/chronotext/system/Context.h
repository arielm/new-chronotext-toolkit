/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Cinder.h"

#if !defined(CINDER_COCOA_TOUCH) && !defined(CINDER_ANDROID) && !defined(CINDER_MAC) && !defined(CINDER_MSW)

#error UNSUPPORTED PLATFORM

#endif

// ---

#include "chronotext/system/SystemManager.h"
#include "chronotext/system/MemoryManager.h"
#include "chronotext/system/DisplayHelper.h"
#include "chronotext/os/TaskManager.h"

#include <boost/asio.hpp>

namespace chronotext
{
    class CinderSketch;
    
    CinderSketch* createSketch();
    void destroySketch(CinderSketch *sketch);
    
    // ---

    namespace context
    {
        system::Manager* systemManager();
        memory::Manager* memoryManager();
        
        boost::asio::io_service& io();
        TaskManager* taskManager();
        
        // ---
        
        bool init();
        
        void setup(boost::asio::io_service &io);
        void shutdown();
    };
}

namespace chr = chronotext;