/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/system/MemoryInfo.h"
#include "chronotext/system/MemoryBuffer.h"

#include <map>

namespace chr
{
    MemoryInfo getMemoryInfo();
    
    namespace memory
    {
        int64_t compare(const Info &before, const Info &after);

        class ManagerBase
        {
        public:
            static std::atomic<bool> LOG_VERBOSE;
            static std::atomic<bool> LOG_WARNING;
            
            virtual void setup() {}
            virtual void shutdown() {}
            
            virtual Info getInfo()
            {
                return Info();
            }
            
            /*
             * RETURNS HOW MUCH MEMORY WAS USED BETWEEN "BEFORE" AND "AFTER"
             */
            virtual int64_t compare(const Info &before, const Info &after)
            {
                return 0;
            }
            
            std::shared_ptr<Buffer> getBuffer(InputSource &inputSource);
            
        protected:
            std::map<std::string, std::shared_ptr<Buffer>> buffers;
        };
    }
}
