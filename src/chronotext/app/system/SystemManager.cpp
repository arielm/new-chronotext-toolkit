/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/app/system/SystemManager.h"
#include "chronotext/Context.h"

#include "cinder/System.h"

using namespace std;
using namespace ci;

namespace chr
{
    namespace system
    {
        void Manager::setup(const BootInfo &bootInfo)
        {
            Manager::bootInfo = bootInfo;

            updateInfo();
            
            LOGI_IF(true) << "SYSTEM INFO: " << getInfo() << endl; // LOG: VERBOSE
            LOGI_IF(true) << "DISPLAY INFO: " << getDisplayInfo() << endl; // LOG: VERBOSE
        }

        const DisplayInfo& Manager::getDisplayInfo() const
        {
            return bootInfo.getDisplayInfo();
        }
        
        string Manager::getIpAddress(bool maskForBroadcast)
        {
            string host = System::getIpAddress();
            
            if (maskForBroadcast)
            {
                if (host.rfind('.') != string::npos)
                {
                    host.replace(host.rfind('.') + 1, 3, "255");
                }
            }
            
            return host;
        }
        
        // ---
        
        string Manager::getOsVersionString()
        {
            return ci::toString(System::getOsMajorVersion()) + "." + ci::toString(System::getOsMinorVersion()) + "." + ci::toString(System::getOsBugFixVersion());
        }
        
        string Manager::getDeviceString()
        {
            return "";
        }
    }
}
