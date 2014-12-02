/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * DUMMY IMPLEMENTATION:
 *
 * 1) NECESSARY PLACEHOLDER ON WINDOWS (NOT IMPLEMENTED YET)
 *
 * 2) NECESSARY BASE FOR "COMPLEX MULTI-PLATFORM INHERITANCE" (CF MemoryManager)
 */

/*
 * DEVELOPED AND TESTED VIA THE ContextRework PROJECT IN THE chronotext-playground REPOSITORY:
 * - https://github.com/arielm/chronotext-playground/blob/e4ec36e252db9e9b759bbaf5439700e76421f7cb/Sketches/ContextRework/src/TestingMemory.h
 *
 *
 * ADDITIONAL PLATFORM-RELATED INFO:
 *
 * - chronotext/cocoa/system/MemoryManager.mm
 * - chronotext/android/system/MemoryManager.cpp
 */

#pragma once

#include "chronotext/system/MemoryInfo.h"

namespace chronotext
{
    namespace memory
    {
        Info getInfo();

        // ---
        
        class ManagerBase
        {
        protected:
            ManagerBase() {}
            ~ManagerBase() {}
        };
    }
}
