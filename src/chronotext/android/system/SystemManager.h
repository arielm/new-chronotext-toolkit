/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/system/SystemManagerBase.h"

namespace chronotext
{
    namespace system
    {
        /*
         * THE SAME PROPERTIES THAT CAN BE QUERIED VIA ADB, E.G.
         * adb shell getprop ro.product.manufacturer
         */
        
        std::string getProperty(const char *name);

        class Manager : public ManagerBase
        {
        public:
            static Manager* instance();
            
            std::string getIpAddress(bool maskForBroadcast = false); // TODO
            
        protected:
            Manager();
            
            void updateInfo();
            
            std::string getOsVersionString();
            std::string getDeviceString();
            
            std::string getModel();
            std::string getManufacturer();
        };
    }
}
