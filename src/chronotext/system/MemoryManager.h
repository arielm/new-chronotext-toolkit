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

#include "chronotext/cocoa/system/MemoryManager.h"

#elif defined(CINDER_ANDROID)

#include "chronotext/android/system/MemoryManager.h"

#else

#include "chronotext/system/MemoryManagerBase.h"

namespace chronotext
{
    namespace memory
    {
        typedef ManagerBase Manager;
    }
}

#endif
