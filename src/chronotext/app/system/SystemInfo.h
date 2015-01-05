/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/system/SystemInfoBase.h"
#include "chronotext/app/system/EmulatedDevice.h"

namespace chr
{
    namespace system
    {
        struct BootInfo
        {
        public:
            bool emulated;
            EmulatedDevice emulatedDevice;
            DisplayInfo actualDisplayInfo;
            
            const DisplayInfo& getDisplayInfo() const
            {
                return emulated ? emulatedDevice.displayInfo : actualDisplayInfo;
            }
        };
    }
}
