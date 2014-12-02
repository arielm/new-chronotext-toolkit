/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * CURRENTLY DUMMY IMPLEMENTATION:
 *
 * 1) NECESSARY PLACEHOLDER ON WINDOWS
 *
 * 2) NECESSARY BASE FOR "COMPLEX MULTI-PLATFORM INHERITANCE" (CF MemoryManager)
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
