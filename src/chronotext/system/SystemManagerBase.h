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
    namespace system
    {
        Platform getPlatform();
        std::string getPlatformString();

        Info getInfo();

        // ---
        
        class ManagerBase
        {
        public:
            Info info;

            static ManagerBase* instance();
            
            virtual std::string getIpAddress(bool maskForBroadcast = false);
            
        protected:
            ManagerBase();
            ~ManagerBase() {}
            
            virtual void updateInfo();
            
            virtual std::string getOsVersionString();
            virtual std::string getDeviceString();
        };
    }
}
