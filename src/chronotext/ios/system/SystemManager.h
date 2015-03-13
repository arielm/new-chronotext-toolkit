/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Cinder.h"

#if !defined(CINDER_COCOA_TOUCH)
#   error UNSUPPORTED PLATFORM
#endif

// ---

#include "chronotext/system/SystemManagerBase.h"

namespace chr
{
    namespace system
    {
        class Manager : public ManagerBase
        {
        public:
            void setup(const InitInfo &initInfo) final;

            std::string getIpAddress(bool maskForBroadcast = false);
            
        protected:
            void updateInfo() final;
            
            std::array<int, 3> getOsVersion() final;
            std::string getOsVersionString() final;
            std::string getDeviceString() final;
            
            std::string getModel();
            std::string getMachine();
            Info::Generation getGeneration();
            
            bool isPodTouch();
            bool isIPhone();
            bool isIPad();
            bool isIPadMini();
            bool isSimulator();
        };
    }
}
