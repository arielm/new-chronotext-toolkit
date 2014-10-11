/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/cinder/WindowInfo.h"
#include "chronotext/system/DisplayInfo.h"

namespace chronotext
{
    struct EmulatedDevice
    {
        DisplayInfo displayInfo;
        WindowInfo windowInfo;
        
        EmulatedDevice()
        {}
        
        EmulatedDevice(const DisplayInfo &displayInfo)
        :
        displayInfo(displayInfo)
        {
            windowInfo.size = displayInfo.getSize();
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
