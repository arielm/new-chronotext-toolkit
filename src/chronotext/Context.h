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
#include "chronotext/os/TaskManager.h"
#include "chronotext/utils/Utils.h"

#include <boost/asio.hpp>

namespace chr
{
    class CinderSketch;
    
    CinderSketch* createSketch();
    void destroySketch(CinderSketch *sketch);
    
    // ---
    
    namespace CONTEXT
    {
        bool init(const system::BootInfo &bootInfo);
        void setup(boost::asio::io_service &io_service);
        void shutdown();
    }
}

namespace context
{
    chr::system::Manager& systemManager();
    chr::memory::Manager& memoryManager();
    chr::TaskManager& taskManager();
    
    chr::SystemInfo getSystemInfo();
    chr::MemoryInfo getMemoryInfo();
    chr::DisplayInfo getDisplayInfo();
    
    namespace os
    {
        /*
         * RETURNS FALSE IF NOT INVOKED ON THE SKETCH-THREAD
         */
        bool isThreadSafe();
        
        /*
         * RETURNS FALSE IF THE FUNCTION CAN'T BE POSTED
         *
         * CAUSES:
         *
         * - IO-SERVICE IS NOT DEFINED
         * - THE CONTEXT IS BEING SHUT-DOWN (TODO)
         *
         * SYNCHRONOUS POSTING ONLY:
         * - NOT INVOKED ON THE SKETCH-THREAD
         */
        bool post(std::function<void()> &&fn, bool forceSync = false);
    }
};
