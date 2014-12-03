/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "SystemManager.h"

#include "chronotext/utils/Utils.h"

#include <sys/system_properties.h>
#include <sys/utsname.h>

using namespace std;
using namespace ci;

namespace chronotext
{
    namespace system
    {
        string getProperty(const char *name)
        {
            static char tmp[256];
            auto len = __system_property_get(name, tmp);
            
            return string(tmp, len);
        }

        // ---
        
        void Manager::updateInfo()
        {
            /*
             * XXX: ORDER OF EXECUTION MATTERS!
             */
            
            info.model = getModel();
            info.manufacturer = getManufacturer();
            
            ManagerBase::updateInfo();
        }
        
        string Manager::getOsVersionString()
        {
            return getProperty("ro.build.version.release");
        }
        
        string Manager::getDeviceString()
        {
            return info.model + " [" + info.manufacturer + "]";
        }
        
        string Manager::getModel()
        {
            return getProperty("ro.product.model");
        }
        
        string Manager::getManufacturer()
        {
            return getProperty("ro.product.manufacturer");
        }
    }
}
