/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/system/DisplayInfo.h"

#if defined(CINDER_MAC) || defined(CINDER_MSW)

#include "cinder/app/AppNative.h"
#include "cinder/app/Renderer.h"

#endif

namespace chronotext
{
    class DisplayHelper
    {
    public:
        static DisplayInfo::SizeFactor getSizeFactor(const DisplayInfo &displayInfo);
        
#if defined(CINDER_MAC) || defined(CINDER_MSW)
        
        static int getAALevel(ci::app::AppNative *application);
        
#endif
    };
}
