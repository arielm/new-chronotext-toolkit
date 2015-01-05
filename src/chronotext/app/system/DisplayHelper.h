/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Cinder.h"

#if !defined(CINDER_MAC) && !defined(CINDER_MSW)

#error UNSUPPORTED PLATFORM

#endif

// ---

#include "chronotext/system/DisplayHelperBase.h"
#include "chronotext/system/SystemInfo.h"

#include "cinder/app/AppNative.h"

namespace chr
{
    namespace display
    {
        class Helper : public HelperBase
        {
        public:
            static void setup(const system::InitInfo &initInfo);
            static void shutdown();
            
            static int getAALevel(ci::app::AppNative *application);
        };
    }
}
