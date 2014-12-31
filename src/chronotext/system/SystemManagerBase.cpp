/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/system/SystemManagerBase.h"
#include "chronotext/Context.h"

using namespace std;
using namespace ci;

namespace context
{
    chr::system::Info getSystemInfo()
    {
        return systemManager().info;
    }
}

namespace chr
{
    namespace system
    {
        Platform platform()
        {
#if defined(CINDER_MAC)
            return PLATFORM_OSX;
#elif defined(CINDER_MSW)
            return PLATFORM_WINDOW;
#elif defined(CINDER_COCOA_TOUCH)
            return PLATFORM_IOS;
#elif defined(CINDER_ANDROID)
            return PLATFORM_ANDROID;
#endif
            assert(false);
        }
        
        const string& platformName()
        {
            static const string PLATFORM_NAMES[4] = {"OSX", "Windows", "iOS", "Android"};
            return PLATFORM_NAMES[platform()];
        }

        // ---
        
        void ManagerBase::setup()
        {
            updateInfo();
            
            LOGI_IF(true) << "SYSTEM INFO: " << info << endl; // LOG: VERBOSE
        }
        
        // ---
        
        void ManagerBase::updateInfo()
        {
            info.platform = platform();
            
            info.platformString = platformName();
            info.osVersionString = getOsVersionString();
            info.deviceString = getDeviceString();
        }
    }
}
