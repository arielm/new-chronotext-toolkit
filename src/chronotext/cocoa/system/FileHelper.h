/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Cinder.h"

#if !defined(CINDER_COCOA)
#   error UNSUPPORTED PLATFORM
#endif

// ---

#include "chronotext/system/FileHelperBase.h"

namespace chr
{
    namespace file
    {
        class Helper : public HelperBase
        {
        public:
            static void setup(const system::InitInfo &initInfo) {}
            static void shutdown() {}
            
            static ci::fs::path getResourcePath();
            static ci::fs::path getResourcePath(const ci::fs::path &relativePath);
        };
    }
}
