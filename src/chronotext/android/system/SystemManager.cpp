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
    
    SystemInfo SystemManager::getSystemInfo()
    {
        return instance().systemInfo;
    }
    
    SystemManager::SystemManager()
    {
        updateSystemInfo();
    }
    
#pragma mark ---------------------------------------- RUNTIME METHODS ----------------------------------------
    
    string SystemManager::getIpAddress(bool maskForBroadcast)
    {
        return "";
    }
    
#pragma mark ---------------------------------------- SystemInfo ----------------------------------------

    void SystemManager::updateSystemInfo()
    {
        /*
         * XXX: ORDER OF EXECUTION MATTERS!
         */
        
        systemInfo.model = getModel();
        systemInfo.manufacturer = getManufacturer();

        SystemManagerBase::updateSystemInfo();
    }
    
    string SystemManager::getOsVersionString()
    {
        return getSystemProperty("ro.build.version.release");
    }
    
    string SystemManager::getCompositeModelString()
    {
        return systemInfo.model + " [" + systemInfo.manufacturer + "]";
    }
    
    string SystemManager::getModel()
    {
        return getSystemProperty("ro.product.model");
    }
    
    string SystemManager::getManufacturer()
    {
        return getSystemProperty("ro.product.manufacturer");
    }
}
