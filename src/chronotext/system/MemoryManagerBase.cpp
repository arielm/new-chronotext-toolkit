/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "MemoryManagerBase.h"

#include "chronotext/Context.h"

using namespace std;

namespace chr
{
    memory::Info memoryInfo()
    {
        return context::memoryManager()->getInfo();
    }
    
    namespace memory
    {
        bool ManagerBase::setup()
        {
            LOGI << "MEMORY INFO: " << getInfo() << endl;
            return true;
        }
        
        void ManagerBase::shutdown()
        {
            LOGI << "MEMORY INFO: " << getInfo() << endl;
        }
    }
}
