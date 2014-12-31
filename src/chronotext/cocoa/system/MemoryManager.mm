/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * MEMORY-MEASUREMENT SEEMS RELIABLE ON IOS, LESS ON OSX
 *
 *
 * FINDINGS:
 *
 * 1) task_info.resident_size IS THE RIGHT WAY TO MEASURE "USED MEMORY", BUT:
 *    - IT SEEMS TO BE PER-THREAD
 *
 * 2) IOS:
 *    - FREE MEMORY:
 *      - WORKS AS INTENDED, BUT:
 *        - NOT PROPERLY UPDATED ONCE APPROACHING "LOW LIMITS" (~8 MB ON IPAD 1)
 *
 * 3) OSX:
 *    - FREE MEMORY:
 *      - TOTALLY CHAOTIC BEHAVIOR (PROBABLY A SIDE-EFFECT OF "MODERN DESKTOP MEMORY MANAGEMENT")
 *    - TOTAL MEMORY:
 *      - NOT ACCURATE AND USELESS
 *
 *
 * TODO:
 *
 * 1) INVESTIGATE task_info.resident_size FURTHER:
 *    - THE PER-THREAD NATURE MAY REQUIRE A SOLUTION INTEGRATED WITH os/TaskManager
 */

/*
 * PREVIOUS TESTABLE MILESTONE:
 *
 * - https://github.com/arielm/new-chronotext-toolkit/blob/ffa9f658e609f13bf969a17fc76beba3dbe22735/src/chronotext/android/system/MemoryManager.cpp
 * - https://github.com/arielm/chronotext-playground/blob/486d4c4ac02a5e471ed5a1b1cc1cee16bc1044fe/Sketches/ContextRework/src/TestingMemory.h
 */

#include "chronotext/cocoa/system/MemoryManager.h"
#include "chronotext/Context.h"

#import <sys/sysctl.h>
#import <mach/host_info.h>
#import <mach/mach_host.h>
#import <mach/task_info.h>
#import <mach/task.h>

using namespace std;

namespace chr
{
    namespace memory
    {
        Manager::Manager()
        {
            setup(); // REMINDER: INVOCATION FROM BASE-CONSTRUCTOR DISCARDS INHERITANCE
        }
        
        Manager::~Manager()
        {
            shutdown(); // REMINDER: INVOCATION FROM BASE-DESTRUCTOR DISCARDS INHERITANCE
        }

        void Manager::setup()
        {
            LOGI_IF(true) << "MEMORY INFO: " << getInfo() << endl; // LOG: VERBOSE
        }
        
        void Manager::shutdown()
        {
            LOGI_IF(true) << "MEMORY INFO: " << getInfo() << endl; // LOG: VERBOSE
        }
        
        // ---
        
        /*
         * REFERENCES:
         *
         * - http://stackoverflow.com/a/23123849/50335
         * - http://miknight.blogspot.co.il/2005/11/resident-set-size-in-mac-os-x.html
         */

        Info Manager::getInfo()
        {
            int mib[6];
            mib[0] = CTL_HW;
            mib[1] = HW_PAGESIZE;
            
            int pagesize;
            size_t length = sizeof(pagesize);
            
            if (sysctl(mib, 2, &pagesize, &length, NULL, 0) < 0)
            {
                fprintf(stderr, "sysctl FAILURE");
                return Info();
            }
            
            // ---
            
            vm_statistics_data_t vmstat;
            mach_msg_type_number_t vmstat_count = HOST_VM_INFO_COUNT;
            
            if (host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vmstat, &vmstat_count) != KERN_SUCCESS)
            {
                fprintf(stderr, "host_statistics FAILURE");
                return Info();
            }
            
            // ---
            
            task_basic_info info;
            mach_msg_type_number_t info_count = TASK_BASIC_INFO_COUNT;
            
            if (task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&info, &info_count) != KERN_SUCCESS)
            {
                fprintf(stderr, "task_info FAILURE");
                return Info();
            }
            
            // ---
            
            int64_t freeMemory = -1;
            int64_t usedMemory = info.resident_size;
            
            if (system::platform() == system::PLATFORM_IOS)
            {
                freeMemory = vmstat.free_count * pagesize;
            }
            
            return Info(freeMemory, usedMemory);
        }
        
        int64_t Manager::compare(const Info &before, const Info &after)
        {
            if ((before.used > 0) && (after.used > 0))
            {
                return after.used - before.used;
            }
   
            return 0;
        }
    }
}
