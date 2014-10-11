/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
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
        enum
        {
            SIZE_FACTOR_UNDEFINED,
            SIZE_FACTOR_PHONE_MINI,
            SIZE_FACTOR_PHONE,
            SIZE_FACTOR_PHONE_BIG,
            SIZE_FACTOR_TABLET_MINI,
            SIZE_FACTOR_TABLET,
            SIZE_FACTOR_TABLET_BIG,
        };
        
        static int getSizeFactor(const DisplayInfo &displayInfo);
        
#if defined(CINDER_MAC) || defined(CINDER_MSW)
        static int getAALevel(ci::app::AppNative *application);
#endif
    };
}
