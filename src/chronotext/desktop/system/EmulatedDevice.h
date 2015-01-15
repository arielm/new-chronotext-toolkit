/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/cinder/WindowInfo.h"
#include "chronotext/system/DisplayInfo.h"

namespace chr
{
    struct EmulatedDevice
    {
        DisplayInfo displayInfo;
        WindowInfo windowInfo;
        
        EmulatedDevice() = default;
        
        EmulatedDevice(const DisplayInfo &displayInfo)
        :
        displayInfo(displayInfo)
        {
            windowInfo.size = displayInfo.size;
        }
        
        EmulatedDevice(const DisplayInfo &displayInfo, const WindowInfo &windowInfo)
        :
        displayInfo(displayInfo),
        windowInfo(windowInfo)
        {}
        
        void rotate()
        {
            displayInfo.rotate();
            windowInfo.rotate();
        }
        
        friend std::ostream& operator<<(std::ostream &lhs, const EmulatedDevice &rhs)
        {
            lhs
            << "{"
            << "displayInfo: " << rhs.displayInfo
            << ", windowInfo: " << rhs.windowInfo
            << "}";
            
            return lhs;
        }
    };
}
