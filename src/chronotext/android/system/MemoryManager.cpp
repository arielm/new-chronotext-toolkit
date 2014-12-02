/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "MemoryManager.h"

#include "chronotext/Log.h"
#include "chronotext/android/cinder/JNI.h"
#include "chronotext/android/cinder/CinderDelegate.h"

#include "cinder/Json.h"

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

namespace chronotext
{
    namespace memory
    {
        /*
         * "FREE MEMORY" SEEMS RELIABLE, BUT IS LIKELY AFFECTED BY "EXTERNAL FACTORS" (E.G. LIBRARY LOADING)
         *
         * TODO:
         *
         * 1) INFER "RATIO" BY MEASURING THE FREE MEMORY AS CLOSE AS APP-STARTUP
         */
        
        Info getInfo()
        {
            /*
             * CURRENT LIMITATION: MUST BE CALLED FROM THE MAIN-THREAD OR THE RENDERER'S THREAD
             *
             * TODO: ADD SUPPORT FOR JAVA-THREAD-ATTACHMENT IN chronotext/os/Task
             */
            
            jstring query = (jstring)context::delegate->callObjectMethodOnJavaListener("getMemoryInfo", "()Ljava/lang/String;");
            
            if (query)
            {
                JNIEnv *env = jvm::env();

                const char *chars = env->GetStringUTFChars(query, nullptr);
                JsonTree memoryInfo(chars);
                env->ReleaseStringUTFChars(query, chars);
                
                auto availMem = memoryInfo["availMem"].getValue<int64_t>();
                auto threshold = memoryInfo["threshold"].getValue<int64_t>();
                auto lowMemory = memoryInfo["lowMemory"].getValue<bool>();

                // ---
                
                int64_t freeMemory = availMem - threshold;
                int64_t usedMemory = -1;
                double ratio = 0;
                
                return Info(freeMemory, usedMemory, ratio);
            }
            
            return Info();
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
