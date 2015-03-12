/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/system/MemoryManagerBase.h"
#include "chronotext/Context.h"

using namespace std;

namespace chr
{
    namespace memory
    {
        atomic<bool> ManagerBase::LOG_VERBOSE (false);
        atomic<bool> ManagerBase::LOG_WARNING (true);

        int64_t compare(const Info &before, const Info &after)
        {
            return memoryManager().compare(before, after);
        }
    }
    
    MemoryInfo getMemoryInfo()
    {
        return memoryManager().getInfo();
    }
}
