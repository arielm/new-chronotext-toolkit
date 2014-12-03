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
        Platform platform();
        std::string platformName();

        Info info();

        // ---
        
        class ManagerBase
        {
        public:
            Info info; // XXX: NOT ACCESSIBLE FROM system::info() IF PROTECTED

            virtual ~ManagerBase() {}
            
            virtual bool init();
            virtual void uninit() {}

            virtual std::string getIpAddress(bool maskForBroadcast = false) = 0;

        protected:
            virtual void updateInfo();
            
            virtual std::string getOsVersionString() = 0;
            virtual std::string getDeviceString() = 0;
        };
    }
}
