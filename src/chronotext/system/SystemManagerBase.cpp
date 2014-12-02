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
    namespace system
    {
        class Manager;
        
        Platform getPlatform()
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
            throw EXCEPTION(Manager, "UNSUPPORTED PLATFORM");
        }
        
        string getPlatformString()
        {
            static const string PLATFORM_NAMES[4] = {"OSX", "Windows", "iOS", "Android"};
            return PLATFORM_NAMES[getPlatform()];
        }
        
#if defined(CINDER_MAC) || defined(CINDER_MSW)
        
        Info getInfo()
        {
            return ManagerBase::instance()->info;
        }
        
#endif
        
        // ---
        
        ManagerBase* ManagerBase::instance()
        {
            static ManagerBase instance; // XXX
            return &instance;
        }
        
        ManagerBase::ManagerBase()
        {
            updateInfo();
        }
        
#pragma mark ---------------------------------------- RUNTIME METHODS ----------------------------------------
        
#if defined(ANDROID)
        
        string ManagerBase::getIpAddress(bool maskForBroadcast)
        {
            return "";
        }
        
#else
        
        string ManagerBase::getIpAddress(bool maskForBroadcast)
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
        
#pragma mark ---------------------------------------- INFO ----------------------------------------
        
        void ManagerBase::updateInfo()
        {
            info.platform = getPlatform();
            info.platformString = getPlatformString();
            
            info.osVersionString = getOsVersionString();
            info.deviceString = getDeviceString();
        }
        
#if defined(ANDROID)
        
        string ManagerBase::getOsVersionString()
        {
            return "";
        }
        
#else
        
        string ManagerBase::getOsVersionString()
        {
            return ci::toString(System::getOsMajorVersion()) + "." + ci::toString(System::getOsMinorVersion()) + "." + ci::toString(System::getOsBugFixVersion());
        }
        
#endif
        
        string ManagerBase::getDeviceString()
        {
            return "";
        }
    }
}
