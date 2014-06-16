/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/system/SystemInfoBase.h"

namespace chronotext
{
    class SystemInfoImplCocoaTouch : public SystemInfoBase
    {
    private:
        SystemInfoImplCocoaTouch() {}
        
    public:
        static SystemInfoImplCocoaTouch& instance()
        {
            static SystemInfoImplCocoaTouch instance;
            return instance;
        }
        
        int getSizeFactor();
        std::string getModel();
        std::string getManufacturer();
        std::string getPlatform();
    };
}

namespace chr = chronotext;
