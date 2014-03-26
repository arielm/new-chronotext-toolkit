/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "SystemInfoImplCocoaTouch.h"

#include <boost/algorithm/string/predicate.hpp>

#include <sys/utsname.h>

using namespace std;

namespace chronotext
{
    int SystemInfoImplCocoaTouch::getKind()
    {
        auto model = getModel();
        
        if (boost::starts_with(model, "iPhone"))
        {
            return KIND_IPHONE;
        }
        if (boost::starts_with(model, "iPod Touch"))
        {
            return KIND_IPOD_TOUCH;
        }
        if (boost::starts_with(model, "iPad mini"))
        {
            return KIND_IPAD_MINI;
        }
        if (boost::starts_with(model, "iPad"))
        {
            return KIND_IPAD;
        }
        if (model == "iPhone (Simulator)")
        {
            return KIND_IPHONE_SIMULATOR;
        }
        if (model == "iPad (Simulator)")
        {
            return KIND_IPAD_SIMULATOR;
        }
        
        return KIND_UNDEFINED;
    }
    
    /*
     * REFERENCE: http://theiphonewiki.com/wiki/Models
     */
    string SystemInfoImplCocoaTouch::getModel()
    {
        struct utsname systemInfo;
        uname(&systemInfo);
        
        string machine(systemInfo.machine);
        
        if (machine == "iPhone1,1") return "iPhone 1G";
        if (machine == "iPhone1,2") return "iPhone 3G";
        if (machine == "iPhone2,1") return "iPhone 3GS";
        if (machine == "iPhone3,1") return "iPhone 4 (GSM)";
        if (machine == "iPhone3,2") return "iPhone 4 (GSM Rev A)";
        if (machine == "iPhone3,3") return "iPhone 4 (CDMA)";
        if (machine == "iPhone4,1") return "iPhone 4S";
        if (machine == "iPhone5,1") return "iPhone 5 (GSM)";
        if (machine == "iPhone5,2") return "iPhone 5 (GSM+CDMA)";
        if (machine == "iPhone5,3") return "iPhone 5c (GSM)";
        if (machine == "iPhone5,4") return "iPhone 5c (Global)";
        if (machine == "iPhone6,1") return "iPhone 5s (GSM)";
        if (machine == "iPhone6,2") return "iPhone 5s (Global)";
        if (machine == "iPod1,1") return "iPod Touch 1G";
        if (machine == "iPod2,1") return "iPod Touch 2G";
        if (machine == "iPod3,1") return "iPod Touch 3G";
        if (machine == "iPod4,1") return "iPod Touch 4G";
        if (machine == "iPod5,1") return "iPod Touch 5G";
        if (machine == "iPad1,1") return "iPad 1G";
        if (machine == "iPad2,1") return "iPad 2 (Wi-Fi)";
        if (machine == "iPad2,2") return "iPad 2 (GSM)";
        if (machine == "iPad2,3") return "iPad 2 (CDMA)";
        if (machine == "iPad2,4") return "iPad 2 (Wi-Fi Rev A)";
        if (machine == "iPad2,5") return "iPad mini 1G (Wi-Fi)";
        if (machine == "iPad2,6") return "iPad mini 1G (GSM)";
        if (machine == "iPad2,7") return "iPad mini 1G (GSM+CDMA)";
        if (machine == "iPad3,1") return "iPad 3 (Wi-Fi)";
        if (machine == "iPad3,2") return "iPad 3 (GSM+CDMA)";
        if (machine == "iPad3,3") return "iPad 3 (GSM)";
        if (machine == "iPad3,4") return "iPad 4 (Wi-Fi)";
        if (machine == "iPad3,5") return "iPad 4 (GSM)";
        if (machine == "iPad3,6") return "iPad 4 (GSM+CDMA)";
        if (machine == "iPad4,1") return "iPad Air 5G (Wi-Fi)";
        if (machine == "iPad4,2") return "iPad Air 5G (Cellular)";
        if (machine == "iPad4,4") return "iPad mini 2G (Wi-Fi)";
        if (machine == "iPad4,5") return "iPad mini 2G (Cellular)";
        
        if ((machine == "i386") || (machine == "x86_64"))
        {
            if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad)
            {
                return "iPad (Simulator)";
            }
            else
            {
                return "iPhone (Simulator)";
            }
        }
        
        return "?";
    }
    
    string SystemInfoImplCocoaTouch::getManufacturer()
    {
        return "Apple";
    }
    
    string SystemInfoImplCocoaTouch::getPlatform()
    {
        return "iOS";
    }
}
