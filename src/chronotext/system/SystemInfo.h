/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Cinder.h"

#include <string>
#include <iostream>

namespace chronotext
{
    class SystemInfo
    {
    public:
        enum
        {
            PLATFORM_OSX,
            PLATFORM_WINDOW,
            PLATFORM_IOS,
            PLATFORM_ANDROID
        };
        
        int platform;
        std::string platformString;

        std::string osVersion;
        std::string manufacturer;
        std::string model;
        
        friend std::ostream& operator<<(std::ostream &lhs, const SystemInfo &rhs)
        {
            lhs
            << "{"
            << "platform: " << rhs.platformString
            << ", osVersion: " << rhs.osVersion;
            
            if (!rhs.manufacturer.empty())
            {
                lhs << ", manufacturer: " << rhs.manufacturer;
            }
            
            if (!rhs.model.empty())
            {
                lhs << ", model: " << rhs.model;
            }
            
            lhs << "}";
            
            return lhs;
        }
    };
}

namespace chr = chronotext;
