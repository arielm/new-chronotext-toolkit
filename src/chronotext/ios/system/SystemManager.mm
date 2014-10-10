/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "SystemManager.h"

#include "chronotext/utils/Utils.h"

#include <sys/utsname.h>

using namespace std;
using namespace ci;

namespace chronotext
{
    SystemManager& SystemManager::instance()
    {
        static SystemManager instance;
        return instance;
    }
    
    SystemManager::SystemManager()
    {
        updateSystemInfo();
    }
    
#pragma mark ---------------------------------------- SystemInfo ----------------------------------------

    void SystemManager::updateSystemInfo()
    {
        SystemManagerBase::updateSystemInfo();
    }
    
    string SystemManager::getCompositeModelString()
    {
        return getModel() + " [" + getMachine() + "]";
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
    string SystemManager::getModel()
    {
        NSString *model = UIDevice.currentDevice.model;
        return [model UTF8String];
    }
    
    string SystemManager::getMachine()
    {
        struct utsname systemInfo;
        uname(&systemInfo);
        
        return string(systemInfo.machine);
    }
    
    bool SystemManager::isPodTouch()
    {
        return boost::starts_with(getModel() , "iPod touch");
    }
    
    bool SystemManager::isIPhone()
    {
        return boost::starts_with(getModel() , "iPhone");
    }
    
    bool SystemManager::isPad()
    {
        return boost::starts_with(getModel() , "iPad");
    }
    
    /*
     * REFERENCE: http://theiphonewiki.com/wiki/Models
     */
    bool SystemManager::isPadMini()
    {
        string machine = getMachine();
        
        if (machine == "iPad2,5") return true;
        if (machine == "iPad2,6") return true;
        if (machine == "iPad2,7") return true;
        if (machine == "iPad4,4") return true;
        if (machine == "iPad4,5") return true;
        if (machine == "iPad4,6") return true;
        
        return false;
    }
    
    bool SystemManager::isSimulator()
    {
        return boost::ends_with(getModel() , "Simulator");
    }
}
