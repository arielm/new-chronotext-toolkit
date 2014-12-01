/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/system/SystemInfo.h"

namespace chronotext
{
    class SystemManagerBase
    {
    public:
        static SystemInfo::Platform getPlatform();
        static std::string getPlatformString();
        
        static SystemManagerBase& instance();
        static SystemInfo getSystemInfo();

        virtual std::string getIpAddress(bool maskForBroadcast = false);
        
    protected:
        SystemInfo systemInfo;

        SystemManagerBase();
        ~SystemManagerBase() {}
        
        virtual void updateSystemInfo();

        virtual std::string getOsVersionString();
        virtual std::string getDeviceString();
    };
}
