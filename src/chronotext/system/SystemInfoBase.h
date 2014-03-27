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
            SIZE_FACTOR_UNDEFINED,
            SIZE_FACTOR_PHONE_MINI,
            SIZE_FACTOR_PHONE,
            SIZE_FACTOR_PHONE_BIG,
            SIZE_FACTOR_TABLET_MINI,
            SIZE_FACTOR_TABLET,
            SIZE_FACTOR_TABLET_BIG,
        };
        
        void setWindowInfo(const WindowInfo &windowInfo)
        {
            mWindowInfo = windowInfo;
        }
        
        virtual WindowInfo getWindowInfo()
        {
            return mWindowInfo;
        }

        virtual int getSizeFactor()
        {
            if (mWindowInfo.diagonal < 3.33f)
            {
                return SIZE_FACTOR_PHONE_MINI;
            }
            if (mWindowInfo.diagonal < 4.5f)
            {
                return SIZE_FACTOR_PHONE;
            }
            if (mWindowInfo.diagonal < 6.5f)
            {
                return SIZE_FACTOR_PHONE_BIG;
            }
            if (mWindowInfo.diagonal < 9)
            {
                return SIZE_FACTOR_TABLET_MINI;
            }
            if (mWindowInfo.diagonal < 11.5f)
            {
                return SIZE_FACTOR_TABLET;
            }
            
            return SIZE_FACTOR_TABLET_BIG;
        }
        
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
    };
}

namespace chr = chronotext;
