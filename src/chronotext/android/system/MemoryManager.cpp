/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "MemoryManager.h"

#include "chronotext/android/cinder/JNI.h"
#include "chronotext/android/cinder/CinderDelegate.h"
#include "chronotext/Context.h"

/*
 * TODO:
 *
 * 1) SEE TODO'S IN getInfo()
 *
 * 2) IMPLEMENT "AUTOMATIC MEMORY WARNING":
 *    - OPTION 1:
 *      - VIA ActivityManager.MemoryInfo STRUCTURE:
 *        - http://developer.android.com/reference/android/app/ActivityManager.MemoryInfo.html
 *    - OPTION 2:
 *      - VIA ComponentCallbacks2.onTrimMemory() CALLBACK:
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

/*
 * PREVIOUS TESTABLE MILESTONE: USING C++ FOR QUERYING "FREE MEMORY" INSTEAD OF JAVA
 *
 * - https://github.com/arielm/new-chronotext-toolkit/blob/ffa9f658e609f13bf969a17fc76beba3dbe22735/src/chronotext/android/system/MemoryManager.cpp
 * - https://github.com/arielm/chronotext-playground/blob/486d4c4ac02a5e471ed5a1b1cc1cee16bc1044fe/Sketches/ContextRework/src/TestingMemory.h
 */

using namespace std;
using namespace ci;

namespace chr
{
    namespace memory
    {
        Manager::Manager()
        {
            setup();
        }
        
        Manager::~Manager()
        {
            shutdown();
        }
        
        // ---
        
        /*
         * "FREE MEMORY" SEEMS RELIABLE, BUT IS LIKELY AFFECTED BY "EXTERNAL FACTORS" (E.G. LIBRARY LOADING)
         *
         * TODO:
         *
         * 1) COMPARISON BETWEEN 2 MemoryInfo VALUES (DONE IN memory::Manager, PER PLATFORM, INSTEAD OF IN MemoryInfo)
         *
         * 2) IMPLEMENT MemoryInfo::warningLevel
         *
         * 3) MEASURE DURING update AND "WARN" CinderDelegate WHEN NECESSARY
         */
        
        Info Manager::getInfo()
        {
            int64_t freeMemory = -1;
            int64_t usedMemory = -1;
            
            try
            {
                const JsonTree &query = CONTEXT::delegate->jsonQuery("getMemoryInfo");
                
                auto availMem = query["availMem"].getValue<int64_t>();
                auto threshold = query["threshold"].getValue<int64_t>();
                auto lowMemory = query["lowMemory"].getValue<bool>();
                
                // ---
                
                freeMemory = availMem - threshold;
            }
            catch (exception &e)
            {}
            
            return Info(freeMemory, usedMemory);
        }
    }
}
