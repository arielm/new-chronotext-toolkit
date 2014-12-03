/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "SystemManager.h"

#include "chronotext/utils/Utils.h"

#include "cinder/System.h"

using namespace std;
using namespace ci;

namespace chronotext
{
    namespace system
    {
        Manager::Manager()
        {
            init();
        }
        
        Manager::~Manager()
        {
            uninit();
        }

        // ---
        
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
