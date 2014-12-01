/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "SystemManagerBase.h"

#include "chronotext/utils/Utils.h"

#if !defined(ANDROID)

#include "cinder/System.h"

#endif

using namespace std;
using namespace ci;

namespace chronotext
{
    class SystemManager;

    SystemInfo::Platform SystemManagerBase::getPlatform()
    {
#if defined(CINDER_MAC)
        return SystemInfo::Platform::OSX;
#elif defined(CINDER_MSW)
        return SystemInfo::Platform::WINDOW;
#elif defined(CINDER_COCOA_TOUCH)
        return SystemInfo::Platform::IOS;
#elif defined(CINDER_ANDROID)
        return SystemInfo::Platform::ANDROID;
#endif
        throw EXCEPTION(SystemManager, "UNSUPPORTED PLATFORM");
    }
    
    string SystemManagerBase::getPlatformString()
    {
        static const string PLATFORM_NAMES[4] = {"OSX", "Windows", "iOS", "Android"};
        return PLATFORM_NAMES[getPlatform()];
    }
    
    // ---

    SystemManagerBase& SystemManagerBase::instance()
    {
        static SystemManagerBase instance;
        return instance;
    }
    
    SystemManagerBase::SystemManagerBase()
    {
        updateSystemInfo();
    }

    SystemInfo SystemManagerBase::getSystemInfo()
    {
        return instance().systemInfo;
    }

#pragma mark ---------------------------------------- RUNTIME METHODS ----------------------------------------
    
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
    
#pragma mark ---------------------------------------- SystemInfo ----------------------------------------
    
    void SystemManagerBase::updateSystemInfo()
    {
        systemInfo.platform = getPlatform();
        systemInfo.platformString = getPlatformString();

        systemInfo.osVersionString = getOsVersionString();
        systemInfo.deviceString = getDeviceString();
    }
    
#if defined(ANDROID)
    
    string SystemManagerBase::getOsVersionString()
    {
        return "";
    }
    
#else
    
    string SystemManagerBase::getOsVersionString()
    {
        return ci::toString(System::getOsMajorVersion()) + "." + ci::toString(System::getOsMinorVersion()) + "." + ci::toString(System::getOsBugFixVersion());
    }
    
#endif
    
    string SystemManagerBase::getDeviceString()
    {
        return "";
    }
}
