/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/system/SystemManagerBase.h"

#include <sys/system_properties.h>

namespace chronotext
{
    class SystemManager : public SystemManagerBase
    {
    public:
        /*
         * THE SAME PROPERTIES THAT CAN BE QUERIED VIA ADB, E.G.
         * adb shell getprop ro.product.manufacturer
         */
        static std::string getSystemProperty(const char *name)
        {
            static char tmp[256];
            auto len = __system_property_get(name, tmp);
            
            return std::string(tmp, len);
        }
        
        static SystemManager& instance();
        static SystemInfo getSystemInfo();
        
        std::string getIpAddress(bool maskForBroadcast = false); // TODO

    protected:
        SystemManager();
        void updateSystemInfo();
        
        std::string getOsVersionString();
        std::string getDeviceString();
        
        std::string getModel();
        std::string getManufacturer();
    };
}
