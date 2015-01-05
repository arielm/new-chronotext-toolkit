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
    class DisplayInfo;
    
    namespace system
    {
        Platform platform();
        const std::string& platformName();

        // ---
        
        class ManagerBase
        {
        public:
            virtual void setup(const BootInfo &bootInfo) = 0;
            virtual void shutdown() {}

            const SystemInfo& getInfo() const;
            virtual const DisplayInfo& getDisplayInfo() const = 0;

            virtual std::string getIpAddress(bool maskForBroadcast = false) = 0;

        protected:
            SystemInfo info;
            
            virtual void updateInfo();
            
            virtual std::string getOsVersionString() = 0;
            virtual std::string getDeviceString() = 0;
        };
    }
}
