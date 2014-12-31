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

#include <sys/utsname.h>

using namespace std;
using namespace ci;

namespace chr
{
    namespace system
    {
        Manager::Manager()
        {
            setup();
        }
        
        Manager::~Manager()
        {
            shutdown();
        }
        
        // ---
        
        string Manager::getIpAddress(bool maskForBroadcast)
        {
            return "";
        }
        
        // ---

        void Manager::updateInfo()
        {
            /*
             * XXX: ORDER OF EXECUTION MATTERS!
             */
            
            info.model = getModel();
            info.machine = getMachine();
            
            info.isPodTouch = isPodTouch();
            info.isIPhone = isIPhone();
            info.isIPad = isIPad();
            info.isIPadMini = isIPadMini();
            info.isSimulator = isSimulator();
            
            ManagerBase::updateInfo();
        }
        
        string Manager::getOsVersionString()
        {
            return ci::toString(System::getOsMajorVersion()) + "." + ci::toString(System::getOsMinorVersion()) + "." + ci::toString(System::getOsBugFixVersion());
        }
        
        string Manager::getDeviceString()
        {
            return info.model + " [" + info.machine + "]";
        }
        
        /*
         * SHOULD RETURN ONE THE FOLLOWING:
         *
         * - iPod touch
         * - iPhone
         * - iPhone Simulator
         * - iPad
         * - iPad Simulator
         */
        
        string Manager::getModel()
        {
            NSString *model = UIDevice.currentDevice.model;
            return [model UTF8String];
        }
        
        string Manager::getMachine()
        {
            struct utsname systemInfo;
            uname(&systemInfo);
            
            return string(systemInfo.machine);
        }
        
        bool Manager::isPodTouch()
        {
            return boost::starts_with(info.model , "iPod touch");
        }
        
        bool Manager::isIPhone()
        {
            return boost::starts_with(info.model , "iPhone");
        }
        
        bool Manager::isIPad()
        {
            return boost::starts_with(info.model , "iPad");
        }
        
        /*
         * REFERENCE: http://theiphonewiki.com/wiki/Models
         */
        
        bool Manager::isIPadMini()
        {
            if (info.machine == "iPad2,5") return true;
            if (info.machine == "iPad2,6") return true;
            if (info.machine == "iPad2,7") return true;
            if (info.machine == "iPad4,4") return true;
            if (info.machine == "iPad4,5") return true;
            if (info.machine == "iPad4,6") return true;
            
            return false;
        }
        
        bool Manager::isSimulator()
        {
            return boost::ends_with(info.model , "Simulator");
        }
    }
}
