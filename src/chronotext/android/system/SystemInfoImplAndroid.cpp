/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "SystemInfoImplAndroid.h"

#include <sys/system_properties.h>

using namespace std;

static char tmp[256];

namespace chronotext
{
    /*
     * SYSTEM PROPERTIES CAN BE QUERIED VIA ADB, E.G.
     * adb shell getprop ro.product.manufacturer
     */
    static string getSystemProperty(const char *name)
    {
        int len = __system_property_get(name, tmp);
        return string(tmp, len);
    }
    
    int SystemInfoImplAndroid::getSizeFactor()
    {
        if (mWindowInfo.diagonal < 3.5f)
        {
            return SIZE_ANDROID_PHONE_MINI;
        }
        if (mWindowInfo.diagonal < 6.5f)
        {
            return SIZE_ANDROID_PHONE;
        }
        if (mWindowInfo.diagonal < 9)
        {
            return SIZE_ANDROID_TABLET_MINI;
        }
        if (mWindowInfo.diagonal < 11.5f)
        {
            return SIZE_ANDROID_TABLET;
        }
        
        return SIZE_ANDROID_TABLET_HUGE;
    }
    
    string SystemInfoImplAndroid::getModel()
    {
        return getSystemProperty("ro.product.model");
    }
    
    string SystemInfoImplAndroid::getManufacturer()
    {
        return getSystemProperty("ro.product.manufacturer");
    }
    
    string SystemInfoImplAndroid::getPlatform()
    {
        return "Android";
    }
    
    string SystemInfoImplAndroid::getOsVersion()
    {
        return getSystemProperty("ro.build.version.release");
    }
    
    string SystemInfoImplAndroid::getIpAddress(bool maskForBroadcast)
    {
        return ""; // TODO
    }
}
