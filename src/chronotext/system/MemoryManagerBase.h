/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * TESTED AND DEVELOPED VIA THE TestBed1 PROJECT IN THE chronotext-playgound REPOSITORY:
 *
 * - https://github.com/arielm/chronotext-playground/blob/master/Sketches/TestBed1/src/TestingMemory1.cpp
 * - https://github.com/arielm/chronotext-playground/blob/master/Sketches/TestBed1/src/TestingMemory2.cpp
 *
 *
 * ADDITIONAL PLATFORM-RELATED INFO:
 *
 * - https://github.com/arielm/new-chronotext-toolkit/blob/develop/src/chronotext/cocoa/system/MemoryManager.mm
 * - https://github.com/arielm/new-chronotext-toolkit/blob/develop/src/chronotext/android/system/MemoryManager.cpp
 */

#pragma once

#include "chronotext/system/MemoryInfo.h"

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
            
            virtual Info getInfo() { return Info(); }
            
            /*
             * RETURNS HOW MUCH MEMORY WAS USED BETWEEN "BEFORE" AND "AFTER"
             */
            virtual int64_t compare(const Info &before, const Info &after) { return 0; }
        };
    }
}
