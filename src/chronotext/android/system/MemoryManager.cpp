/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "MemoryManager.h"

#include "chronotext/android/cinder/JNI.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * TODO:
 *
 * 1) SEE TODO'S IN getInfo()
 *
 * 2) IMPLEMENT "AUTOMATIC MEMORY WARNING":
 *    - OPTION 1:
 *      - VIA ANDROID'S MemoryInfo STRUCTURE:
 *        - http://developer.android.com/reference/android/app/ActivityManager.MemoryInfo.html
 *    - OPTION 2:
 *      - VIA ANDROID'S onTrimMemory() CALLBACK:
 *        - http://developer.android.com/reference/android/content/ComponentCallbacks2.html
 *
 *
 * ADDITIONAL REFERENCES:
 *
 * - http://stackoverflow.com/a/18894037/50335
 * - http://grepcode.com/file/repository.grepcode.com/java/ext/com.google.android/android/4.1.1_r1/com/android/server/am/ActivityManagerService.java#5404
 * - http://grepcode.com/file/repository.grepcode.com/java/ext/com.google.android/android/4.1.1_r1/com/android/server/am/ProcessList.java#ProcessList.getMemLevel%28int%29
 * - http://grepcode.com/file/repository.grepcode.com/java/ext/com.google.android/android/4.1.1_r1/com/android/internal/util/MemInfoReader.java
 *
 *
 * TESTABLE VIA TERMINAL WHEN ANDROID DEVICE IS CONNECTED:
 *
 * - adb shell cat /proc/meminfo
 * - adb shell cat /sys/module/lowmemorykiller/parameters/minfree
 */

namespace chronotext
{
    namespace memory
    {
        /*
         * SOURCE: https://android.googlesource.com/platform/frameworks/base/+/master/core/jni/android_util_Process.cpp
         */
        
        static jlong getFreeMemoryImpl(const char* const sums[], const size_t sumsLen[], size_t num)
        {
            int fd = open("/proc/meminfo", O_RDONLY);
            if (fd < 0) {
                fprintf(stderr, "UNABLE TO OPEN /proc/meminfo"); // XXX
                return -1;
            }
            char buffer[256];
            const int len = read(fd, buffer, sizeof(buffer)-1);
            close(fd);
            if (len < 0) {
                fprintf(stderr, "UNABLE TO READ /proc/meminfo"); // XXX
                return -1;
            }
            buffer[len] = 0;
            size_t numFound = 0;
            jlong mem = 0;
            char* p = buffer;
            while (*p && numFound < num) {
                int i = 0;
                while (sums[i]) {
                    if (strncmp(p, sums[i], sumsLen[i]) == 0) {
                        p += sumsLen[i];
                        while (*p == ' ') p++;
                        char* num = p;
                        while (*p >= '0' && *p <= '9') p++;
                        if (*p != 0) {
                            *p = 0;
                            p++;
                            if (*p == 0) p--;
                        }
                        mem += atoll(num) * 1024;
                        numFound++;
                        break;
                    }
                    i++;
                }
                p++;
            }
            return numFound > 0 ? mem : -1;
        }
        
        static jlong getFreeMemory()
        {
            static const char* const sums[] = { "MemFree:", "Cached:", NULL };
            static const size_t sumsLen[] = { strlen("MemFree:"), strlen("Cached:"), 0 };
            return getFreeMemoryImpl(sums, sumsLen, 2);
        }
        
        static jlong getTotalMemory()
        {
            static const char* const sums[] = { "MemTotal:", NULL };
            static const size_t sumsLen[] = { strlen("MemTotal:"), 0 };
            return getFreeMemoryImpl(sums, sumsLen, 1);
        }

        Info getInfo()
        {
            /*
             * "FREE MEMORY" SEEMS RELIABLE, BUT IS LIKELY AFFECTED BY "EXTERNAL FACTORS" (E.G. LIBRARY LOADING)
             *
             * TODO:
             *
             * 1) FIND A WAY TO GRASP "USED MEMORY"
             *
             * 2) DECIDE IF "TOTAL MEMORY" WORTHS BEING USED
             *    - CURRENTLY: NOT USEFUL
             */
            
            int64_t freeMemory = getFreeMemory();
            int64_t totalMemory = getTotalMemory();
            
            return Info(freeMemory, totalMemory);
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
