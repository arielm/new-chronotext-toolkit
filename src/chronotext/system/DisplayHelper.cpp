/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/system/DisplayHelper.h"

using namespace std;
using namespace ci;

namespace chr
{
    DisplayInfo::SizeFactor DisplayHelper::getSizeFactor(const DisplayInfo &displayInfo)
    {
        float diagonal = displayInfo.diagonal;
        
        if (diagonal > 0)
        {
            if (diagonal < 3.33f)
            {
                return DisplayInfo::SIZE_FACTOR_PHONE_MINI;
            }
            if (diagonal < 4.5f)
            {
                return DisplayInfo::SIZE_FACTOR_PHONE;
            }
            if (diagonal < 6.5f)
            {
                return DisplayInfo::SIZE_FACTOR_PHONE_BIG;
            }
            if (diagonal < 9)
            {
                return DisplayInfo::SIZE_FACTOR_TABLET_MINI;
            }
            if (diagonal < 11.5f)
            {
                return DisplayInfo::SIZE_FACTOR_TABLET;
            }
            
            return DisplayInfo::SIZE_FACTOR_TABLET_BIG;
        }
        
        return DisplayInfo::SIZE_FACTOR_UNDEFINED;
    }
    
#if defined(CINDER_MAC) || defined(CINDER_MSW)
    
    int DisplayHelper::getAALevel(app::AppNative *application)
    {
        auto renderer = static_pointer_cast<app::RendererGl>(application->getRenderer());

        switch (renderer->getAntiAliasing())
        {
            case app::RendererGl::AA_MSAA_2:
                return 2;
                
            case app::RendererGl::AA_MSAA_4:
                return 4;
                
            case app::RendererGl::AA_MSAA_6:
                return 6;
                
            case app::RendererGl::AA_MSAA_8:
                return 8;
                
            case app::RendererGl::AA_MSAA_16:
                return 16;
                
            case app::RendererGl::AA_MSAA_32:
                return 32;
        }
        
        return 0;
    }
    
#endif
}
