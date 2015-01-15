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

#include "chronotext/cinder/CinderSketch.h"
#include "chronotext/system/SystemManager.h"
#include "chronotext/system/MemoryManager.h"
#include "chronotext/os/TaskManager.h"
#include "chronotext/utils/Utils.h"

namespace chr
{
    CinderSketch* createSketch();
    void destroySketch(CinderSketch *sketch);
    
    // ---
    
    SystemManager& systemManager();
    MemoryManager& memoryManager();
    TaskManager& taskManager();

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
    
    template <class T>
    inline T& checkedReference(T *instance)
    {
        assert(instance);
        return *instance;
    }
    
    // ---
    
    namespace INTERN
    {
        bool init(const system::InitInfo &initInfo);
        void launch(const system::LaunchInfo &launchInfo);
        void setup(const system::SetupInfo &setupInfo);
        void shutdown();
    }
}
