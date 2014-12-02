/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "MemoryManager.h"

#include "chronotext/system/SystemManager.h"

#import <sys/sysctl.h>
#import <mach/host_info.h>
#import <mach/mach_host.h>
#import <mach/task_info.h>
#import <mach/task.h>

namespace chronotext
{
    namespace memory
    {
        /*
         * REFERENCES:
         *
         * - http://stackoverflow.com/a/23123849/50335
         * - http://miknight.blogspot.co.il/2005/11/resident-set-size-in-mac-os-x.html
         */

        Info getInfo()
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
            
            /*
             * MEASUREMENTS SEEM RELIABLE ON IOS, BUT NOT ON OSX
             *
             *
             * TODO:
             *
             * 1) INVESTIGATE ON IOS:
             *    "FREE MEMORY":
             *    - NOT PROPERLY UPDATED ANYMORE WHEN APPROACHING "LOW LIMITS" (~8 MB ON IPAD 1)
             *
             * 2) INVESTIGATE ON OSX:
             *    - "FREE MEMORY": TOTALLY CHAOTIC BEHAVIOR (PROBABLY A SIDE-EFFECT OF "MODERN DESKTOP MEMORY MANAGEMENT")
             *    - "USED MEMORY":
             *      - BIGGER BY A FACTOR OF ~3
             *        - COMPARED TO WHAT'S SHOWN IN "INSTRUMENTS" A EXPECTED CONSUMPTION:
             *        - TESTED ON OSX 10.9.5 / 64-BIT BUILD
             *
             * 3) DECIDE IF "TOTAL MEMORY" WORTHS BEING USED
             *    - CURRENTLY:
             *      - PROBABLY NOT ACCURATE
             *      - NOT USEFUL ANYWAY
             */
            
            int64_t freeMemory = vmstat.free_count * pagesize;
            int64_t totalMemory = (vmstat.wire_count + vmstat.active_count + vmstat.inactive_count + vmstat.free_count) * pagesize;
            int64_t usedMemory = info.resident_size;
            
            if (system::getPlatform() == system::PLATFORM_IOS)
            {
                return Info(freeMemory, totalMemory + usedMemory, usedMemory);
            }
            else
            {
                return Info(freeMemory, totalMemory, usedMemory / 3); // XXX: SEE EARLIER COMMENTS
            }
        }
        
        // ---
        
        Manager* Manager::instance()
        {
            static Manager instance; // XXX
            return &instance;
        }
        
        Manager::Manager()
        {}
    }
}
