/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/desktop/system/SystemManager.h"
#include "chronotext/Context.h"

#include "cinder/System.h"

using namespace std;
using namespace ci;

namespace chr
{
    namespace system
    {
        void Manager::setup(const InitInfo &initInfo)
        {
            Manager::initInfo = initInfo;
            
            updateInfo();
            
            LOGI_IF(true) << "SYSTEM INFO: " << info << endl; // LOG: VERBOSE
        }
        
        // ---
        
        string Manager::getIpAddress(bool maskForBroadcast)
        {
            string host = System::getIpAddress();
            
            /*
             * TODO: CONSIDER USING boost::asio::ip
             */
            
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
        
        void Manager::updateInfo()
        {
            info.emulated = initInfo.emulated;
            
            ManagerBase::updateInfo();
        }
        
        array<int, 3> Manager::getOsVersion()
        {
            return {System::getOsMajorVersion(), System::getOsMinorVersion(), System::getOsBugFixVersion()};
        }
        
        string Manager::getOsVersionString()
        {
            auto osVersion = getOsVersion();
            return toString(osVersion[0]) + "." + toString(osVersion[1]) + "." + toString(osVersion[2]);
        }
        
        string Manager::getDeviceString()
        {
            return "";
        }
    }
}
