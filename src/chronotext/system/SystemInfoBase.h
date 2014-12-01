/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Cinder.h"

#include <string>
#include <iostream>

namespace chronotext
{
    class SystemInfoBase
    {
    public:
        enum Platform
        {
            OSX,
            WINDOW,
            IOS,
            ANDROID
        };
        
        Platform platform;
        std::string platformString;

        std::string osVersionString;
        std::string deviceString;
        
        friend std::ostream& operator<<(std::ostream &lhs, const SystemInfoBase &rhs)
        {
            lhs
            << "{"
            << "platform: " << rhs.platformString
            << ", os-version: " << rhs.osVersionString;
            
            if (!rhs.deviceString.empty())
            {
                lhs << ", device: '" << rhs.deviceString << "'";
            }
            
            lhs << "}";
            
            return lhs;
        }
    };
}

namespace chr = chronotext;
