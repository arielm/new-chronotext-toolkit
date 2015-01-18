/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/system/SystemInfoBase.h"
#include "chronotext/desktop/system/EmulatedDevice.h"

namespace chr
{
    namespace system
    {
        struct InitInfo
        {
            bool emulated = false;
            EmulatedDevice emulatedDevice;
            
            float actualContentScale = 0;
            ci::Vec2i actualWindowSize {};
        };
    }
}
