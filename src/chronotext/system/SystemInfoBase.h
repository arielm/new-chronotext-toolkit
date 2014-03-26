/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Cinder.h"
#include "cinder/System.h"
#include "cinder/Utilities.h"

namespace chronotext
{
    class SystemInfoBase
    {
    public:
        enum
        {
            KIND_UNDEFINED,
            KIND_DESKTOP,
            KIND_IPHONE,
            KIND_IPOD_TOUCH,
            KIND_IPAD,
            KIND_IPAD_MINI,
            KIND_IPHONE_SIMULATOR,
            KIND_IPAD_SIMULATOR,
            KIND_ANDROID_PHONE,
            KIND_ANDROID_PHONE_MINI,
            KIND_ANDROID_TABLET,
            KIND_ANDROID_TABLET_MINI,
            KIND_ANDROID_TABLET_HUGE,
        };
        
        virtual int getKind() = 0;
        virtual std::string getModel() = 0;
        virtual std::string getManufacturer() = 0;
        virtual std::string getPlatform() = 0;
        
#if defined(ANDROID)
        virtual std::string getOsVersion() = 0;
        virtual std::string getIpAddress(bool maskForBroadcast = false) = 0;
#else
        virtual std::string getOsVersion()
        {
            return ci::toString(ci::System::getOsMajorVersion()) + "." + ci::toString(ci::System::getOsMinorVersion()) + "." + ci::toString(ci::System::getOsBugFixVersion());
        }
        
        virtual std::string getIpAddress(bool maskForBroadcast = false)
        {
            std::string host = ci::System::getIpAddress();
            
            if (maskForBroadcast)
            {
                if (host.rfind('.') != std::string::npos)
                {
                    host.replace(host.rfind('.') + 1, 3, "255");
                }
            }
            
            return host;
        }
#endif
    };
}

namespace chr = chronotext;
