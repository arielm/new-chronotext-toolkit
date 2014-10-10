/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Cinder.h"

#include "chronotext/system/SystemInfo.h"

namespace chronotext
{
    class SystemManagerBase
    {
    public:
        static int getPlatform();
        static std::string getPlatformString();
        
        static SystemManagerBase& instance()
        {
            static SystemManagerBase instance;
            return instance;
        }

        virtual SystemInfo getSystemInfo() const;
        virtual std::string getIpAddress(bool maskForBroadcast = false);
        
    protected:
        SystemInfo systemInfo;

        SystemManagerBase();
        virtual void update();

        virtual std::string getOsVersionString();
        virtual std::string getCompositeModelString();
    };
}
