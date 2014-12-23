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

#include "chronotext/Log.h"
#include "chronotext/InputSource.h"
#include "chronotext/system/FileHelper.h"
#include "chronotext/system/DisplayHelper.h"

#include <boost/asio.hpp>

namespace chr
{
    class CinderSketch;
    
    CinderSketch* createSketch();
    void destroySketch(CinderSketch *sketch);
    
    // ---
    
    namespace CONTEXT
    {
        bool init();
        void setup(boost::asio::io_service &io_service);
        void shutdown();
    }
}

namespace context
{
    chr::system::Manager* systemManager();
    chr::memory::Manager* memoryManager();
    chr::TaskManager* taskManager();
    
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
};
