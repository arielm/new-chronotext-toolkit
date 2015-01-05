/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/ios/system/DisplayHelper.h"
#include "chronotext/Context.h"

using namespace std;
using namespace ci;

namespace chr
{
    namespace display
    {
        namespace intern
        {
            DisplayInfo displayInfo;
            bool setup = false;
        }
        
        // ---
        
        void Helper::setup(const system::Info &systemInfo)
        {
            if (!intern::setup)
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
                
                intern::displayInfo = DisplayInfo::createWithDiagonal(baseSize.x, baseSize.y, diagonal, contentScale);
                
                // ---
                
                LOGI_IF(true) << "DISPLAY INFO: " << intern::displayInfo << endl; // LOG: VERBOSE
                
                intern::setup = true;
            }
        }
        
        void Helper::shutdown()
        {
            intern::setup = false;
        }
    }
}

namespace context
{
    const chr::DisplayInfo& getDisplayInfo()
    {
        assert(chr::display::intern::setup);
        return chr::display::intern::displayInfo;
    }
}
