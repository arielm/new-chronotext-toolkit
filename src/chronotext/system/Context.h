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

namespace chronotext
{
    class CinderSketch;
    
    CinderSketch* createSketch();
    void destroySketch(CinderSketch *sketch);
    
    // ---

    class Context
    {
    public:
        Context();
        ~Context();
    };

    namespace context
    {
        system::Manager* systemManager();
        
        bool init();
        void uninit();
    };
}

namespace chr = chronotext;
