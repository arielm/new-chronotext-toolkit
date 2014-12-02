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
         * REFERENCE: http://stackoverflow.com/a/6095158/50335
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
            mach_msg_type_number_t count = HOST_VM_INFO_COUNT;
            
            if (host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vmstat, &count) != KERN_SUCCESS)
            {
                fprintf(stderr, "host_statistics FAILURE");
                return Info();
            }
            
            // ---
            
            task_basic_info_64_data_t info;
            unsigned size = sizeof(info);
            
            task_info(mach_task_self(), TASK_BASIC_INFO_64, (task_info_t)&info, &size);
            
            /*
             * MEASUREMENTS SEEM RELIABLE ON IOS, BUT NOT ON OSX (PROBABLY A SIDE-EFFECT OF "MODERN DESKTOP MEMORY MANAGEMENT")
             *
             * TODO:
             *
             * 1) INVESTIGATE ON OSX:
             *    - "FREE AND USED MEMORY": SEEMS TO BE AFFECTED BY OTHER FACTORS THAN WHAT "WE" ALLOCATE:
             *      - MAYBE: LIBRARY LOADING?
             *    - "USED MEMORY": HIGHER THAN WHAT SHOWN BY "INSTRUMENTS"
             *
             * 2) DECIDE IF "TOTAL MEMORY" WORTHS BEING USED
             *    - CURRENTLY: NOT USEFUL
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
                return Info(freeMemory, totalMemory, usedMemory);
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
