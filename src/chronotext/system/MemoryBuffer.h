/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "cinder/Cinder.h"

#if defined(CINDER_COCOA)

#include "chronotext/cocoa/system/MemoryBuffer.h"

#elif defined(CINDER_ANDROID)

#include "chronotext/android/system/MemoryBuffer.h"

#else

#include "chronotext/desktop/system/MemoryBuffer.h"

#endif

namespace chr
{
    typedef memory::Buffer MemoryBuffer;
}
