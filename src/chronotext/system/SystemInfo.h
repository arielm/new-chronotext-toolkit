/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Cinder.h"

#if defined(CHR_COMPLEX) && defined(CINDER_COCOA_TOUCH)
#include "chronotext/ios/system/SystemInfoImplCocoaTouch.h"
namespace chronotext
{
    typedef SystemInfoImplCocoaTouch SystemInfo;
}
#elif defined(CHR_COMPLEX) && defined(CINDER_ANDROID)
#include "chronotext/android/system/SystemInfoImplAndroid.h"
namespace chronotext
{
    typedef SystemInfoImplAndroid SystemInfo;
}
#else
#include "chronotext/system/SystemInfoBase.h"
namespace chronotext
{
    class SystemInfo : public SystemInfoBase
    {
    private:
        SystemInfo() {}
        
    public:
        static SystemInfo& instance()
        {
            static SystemInfo instance;
            return instance;
        }
        
        std::string getModel()
        {
            return "?";
        }
        
        std::string getManufacturer()
        {
#if defined(CINDER_MAC)
            return "Apple";
#endif
            return "?";
        }
        
        std::string getPlatform()
        {
#if defined(CINDER_MAC)
            return "OSX";
#elif defined(CINDER_MSW)
            return "Windows";
#endif
            return "?";
        }
    };
}
#endif

namespace chr = chronotext;
