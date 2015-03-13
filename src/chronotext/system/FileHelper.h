/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Cinder.h"

#if defined(CINDER_COCOA)

#include "chronotext/cocoa/system/FileHelper.h"

#elif defined(CINDER_ANDROID)

#include "chronotext/android/system/FileHelper.h"

#else

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
        };
    }
}

#endif

namespace chr
{
    typedef file::Helper FileHelper;
}
