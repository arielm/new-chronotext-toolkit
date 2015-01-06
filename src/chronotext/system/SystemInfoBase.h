/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/cinder/WindowInfo.h"

#include <boost/asio.hpp>

namespace chr
{
    namespace system
    {
        enum Platform
        {
            PLATFORM_OSX,
            PLATFORM_WINDOW,
            PLATFORM_IOS,
            PLATFORM_ANDROID
        };
        
        struct LaunchInfo
        {
            boost::asio::io_service *io_service;
            
            LaunchInfo() = default;
            
            LaunchInfo(boost::asio::io_service &io_service)
            :
            io_service(&io_service)
            {}
        };
        
        struct SetupInfo
        {
            WindowInfo windowInfo;
            
            SetupInfo() = default;
            
            SetupInfo(const WindowInfo &windowInfo)
            :
            windowInfo(windowInfo)
            {}
        };
        
        class InfoBase
        {
        public:
            Platform platform;
            
            std::string platformString;
            std::string osVersionString;
            std::string deviceString;
            
            friend std::ostream& operator<<(std::ostream &lhs, const InfoBase &rhs)
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
}
