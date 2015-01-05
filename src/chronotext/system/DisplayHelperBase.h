/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/system/DisplayInfo.h"

namespace context
{
    const chr::DisplayInfo& getDisplayInfo();
}

namespace chr
{
    namespace display
    {
        class HelperBase
        {
        public:
            enum SizeFactor
            {
                SIZE_FACTOR_UNDEFINED,
                SIZE_FACTOR_PHONE_MINI,
                SIZE_FACTOR_PHONE,
                SIZE_FACTOR_PHONE_BIG,
                SIZE_FACTOR_TABLET_MINI,
                SIZE_FACTOR_TABLET,
                SIZE_FACTOR_TABLET_BIG,
            };
            
            static SizeFactor getSizeFactor(const Info &displayInfo);
        };
    }
}
