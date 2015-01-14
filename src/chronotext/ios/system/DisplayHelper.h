/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#pragma once

#include "cinder/Cinder.h"

#if !defined(CINDER_COCOA_TOUCH)

#error UNSUPPORTED PLATFORM

#endif

// ---

#include "chronotext/system/DisplayHelperBase.h"
#include "chronotext/system/SystemInfo.h"

namespace chr
{
    namespace display
    {
        class Helper : public HelperBase
        {
        public:
            /*
             * MUST BE CALLED AFTER (OR WITHIN) UIApplication::didFinishLaunchingWithOptions
             * OTHERWISE: SCREEN-ORIENTATION WON'T BE PROPERLY COMPUTED
             */
            static void setup(const system::Info &systemInfo);
            
            static void shutdown();
        };
    }
}
