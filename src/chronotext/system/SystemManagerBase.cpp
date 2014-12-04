/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "SystemManagerBase.h"

#include "chronotext/system/Context.h"
#include "chronotext/utils/Utils.h"

using namespace std;
using namespace ci;

namespace chronotext
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
        
        string platformName()
        {
            static const string PLATFORM_NAMES[4] = {"OSX", "Windows", "iOS", "Android"};
            return PLATFORM_NAMES[platform()];
        }
        
        Info info()
        {
            return context::systemManager()->info;
        }

        // ---
        
        bool ManagerBase::init()
        {
            updateInfo();
            
            LOGI << "SYSTEM INFO: " << info << endl;
            
            return true;
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