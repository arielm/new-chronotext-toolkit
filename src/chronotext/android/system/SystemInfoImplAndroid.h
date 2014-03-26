/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/system/SystemInfoBase.h"
#include "chronotext/cinder/WindowInfo.h"

namespace chronotext
{
    class SystemInfoImplAndroid : public SystemInfoBase
    {
    public:
        int getKind();
        std::string getModel();
        std::string getManufacturer();
        std::string getPlatform();
        std::string getOsVersion();
        std::string getIpAddress(bool maskForBroadcast = false);

        static void setWindowInfo(const WindowInfo &windowInfo);
    };
    
    static WindowInfo gWindowInfo;
}

namespace chr = chronotext;
