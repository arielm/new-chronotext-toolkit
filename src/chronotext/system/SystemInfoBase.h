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

#include "chronotext/cinder/WindowInfo.h"

namespace chronotext
{
    class SystemInfoBase
    {
    public:
        enum
        {
            SIZE_UNDEFINED,
            SIZE_IPHONE,
            SIZE_IPAD_MINI,
            SIZE_IPAD,
            SIZE_ANDROID_PHONE_MINI,
            SIZE_ANDROID_PHONE,
            SIZE_ANDROID_TABLET_MINI,
            SIZE_ANDROID_TABLET,
            SIZE_ANDROID_TABLET_HUGE,
        };
        
        void setWindowInfo(const WindowInfo &windowInfo)
        {
            mWindowInfo = windowInfo;
        }
        
        WindowInfo getWindowInfo() const
        {
            return mWindowInfo;
        }

        void setSizeFactor(int sizeFactor)
        {
            mSizeFactor = sizeFactor;
        }

        virtual int getSizeFactor() = 0;
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
        
    protected:
        WindowInfo mWindowInfo;
        int mSizeFactor;
    };
}

namespace chr = chronotext;
