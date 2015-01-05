/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/system/DisplayHelper.h"
#include "chronotext/Context.h"

using namespace std;
using namespace ci;

using namespace context;

namespace chr
{
    DisplayHelper::SizeFactor DisplayHelper::getSizeFactor(const DisplayInfo &displayInfo)
    {
        float diagonal = displayInfo.diagonal;
        
        if (diagonal > 0)
        {
            if (diagonal < 3.33f)
            {
                return SIZE_FACTOR_PHONE_MINI;
            }
            if (diagonal < 4.5f)
            {
                return SIZE_FACTOR_PHONE;
            }
            if (diagonal < 6.5f)
            {
                return SIZE_FACTOR_PHONE_BIG;
            }
            if (diagonal < 9)
            {
                return SIZE_FACTOR_TABLET_MINI;
            }
            if (diagonal < 11.5f)
            {
                return SIZE_FACTOR_TABLET;
            }
            
            return SIZE_FACTOR_TABLET_BIG;
        }
        
        return SIZE_FACTOR_UNDEFINED;
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
    
#elif defined(CINDER_COCOA_TOUCH)
    
    DisplayInfo DisplayHelper::getDisplayInfo(const system::Info &systemInfo)
    {
        auto bounds = UIScreen.mainScreen.bounds;
        auto contentScale = UIScreen.mainScreen.scale;
        auto orientation = UIApplication.sharedApplication.statusBarOrientation;

        // ---
        
        Vec2i baseSize;
        
        switch (orientation)
        {
            case UIDeviceOrientationUnknown:
            case UIInterfaceOrientationPortrait:
            case UIInterfaceOrientationPortraitUpsideDown:
                baseSize.x = bounds.size.width;
                baseSize.y = bounds.size.height;
                break;
                
            case UIInterfaceOrientationLandscapeLeft:
            case UIInterfaceOrientationLandscapeRight:
                baseSize.x = bounds.size.height;
                baseSize.y = bounds.size.width;
                break;
        }
        
        // ---
        
        float diagonal = 0;
        int magSize = baseSize.x * baseSize.y;
        
        if (magSize == 320 * 480)
        {
            diagonal = 3.54f; // IPHONE 3GS OR 4
        }
        else if (magSize == 320 * 568)
        {
            diagonal = 4.00f; // IPHONE 5
        }
        else if (magSize == 375 * 667)
        {
            diagonal = 4.70f; // IPHONE 6
        }
        else if (magSize == 360 * 640)
        {
            diagonal = 5.50f; // IPHONE 6+
        }
        else if (magSize == 1024 * 768)
        {
            if (systemInfo.isIPadMini)
            {
                diagonal = 7.90f; // IPAD MINI
            }
            else
            {
                diagonal = 9.70f; // IPAD
            }
        }
        
        return DisplayInfo::createWithDiagonal(baseSize.x, baseSize.y, diagonal, contentScale);
    }
    
#endif
}
