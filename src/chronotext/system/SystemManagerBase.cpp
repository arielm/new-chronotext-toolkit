/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "SystemManagerBase.h"

#include "cinder/System.h"
#include "cinder/Utilities.h"

using namespace std;
using namespace ci;

namespace chronotext
{
    int SystemManagerBase::getPlatform()
    {
#if defined(CINDER_MAC)
        return SystemInfo::PLATFORM_OSX;
#elif defined(CINDER_MSW)
        return SystemInfo::PLATFORM_WINDOW;
#elif defined(CINDER_COCOA_TOUCH)
        return SystemInfo::PLATFORM_IOS;
#elif defined(CINDER_ANDROID)
        return SystemInfo::PLATFORM_ANDROID;
#endif
        throw std::runtime_error("UNDEFINED PLATFORM");
    }
    
    string SystemManagerBase::getPlatformString()
    {
        static const string PLATFORM_NAMES[4] = {"OSX", "Windows", "iOS", "Android"};
        return PLATFORM_NAMES[getPlatform()];
    }
    
    SystemManagerBase::SystemManagerBase()
    {
        update();
    }
    
    SystemInfo SystemManagerBase::getSystemInfo() const
    {
        return systemInfo;
    }
    
#if defined(ANDROID)
    string SystemManagerBase::getIpAddress(bool maskForBroadcast)
    {
        return "";
    }
#else
    string SystemManagerBase::getIpAddress(bool maskForBroadcast)
    {
        string host = System::getIpAddress();
        
        if (maskForBroadcast)
        {
            if (host.rfind('.') != string::npos)
            {
                host.replace(host.rfind('.') + 1, 3, "255");
            }
        }
        
        return host;
    }
#endif
    
    void SystemManagerBase::update()
    {
        systemInfo.platform = getPlatform();
        systemInfo.platformString = getPlatformString();

        systemInfo.osVersionString = getOsVersionString();
        systemInfo.compositeModelString = getCompositeModelString();
    }
    
#if defined(ANDROID)
    string SystemManagerBase::getOsVersionString()
    {
        return "";
    }
#else
    string SystemManagerBase::getOsVersionString()
    {
        return toString(System::getOsMajorVersion()) + "." + toString(System::getOsMinorVersion()) + "." + toString(System::getOsBugFixVersion());
    }
#endif
    
    string SystemManagerBase::getCompositeModelString()
    {
        return "";
    }
}
