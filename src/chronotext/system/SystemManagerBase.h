/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/system/SystemInfo.h"

namespace chr
{
    SystemInfo getSystemInfo();
    
    namespace system
    {
        Platform platform();
        const std::string& platformName();

        // ---
        
        class ManagerBase
        {
        public:
            virtual void setup(const InitInfo &initInfo) = 0;
            virtual void shutdown() {}

            virtual std::string getIpAddress(bool maskForBroadcast = false) = 0;

            const Info& getInfo() const;

        protected:
            Info info;
            
            virtual void updateInfo();

            virtual std::array<int, 3> getOsVersion() = 0;
            virtual std::string getOsVersionString() = 0;
            virtual std::string getDeviceString() = 0;
        };
    }
}
