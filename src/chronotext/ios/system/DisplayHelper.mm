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
        
        /*
         * FIXME: SCREEN-SIZE EVALUATION ON IPHONE 6 IS MORE COMPLICATED...
         * https://developer.apple.com/library/ios/releasenotes/General/WhatsNewIniOS/Articles/iOS8.html#//apple_ref/doc/uid/TP40014205-SW46
         */
        
        void Helper::setup(const system::InitInfo &initInfo)
        {
            if (!intern::setup)
            {
                auto bounds = UIScreen.mainScreen.bounds;
                auto contentScale = UIScreen.mainScreen.scale;
                auto orientation = UIApplication.sharedApplication.statusBarOrientation;
                
                // ---
                
                Vec2i baseSize;
                
                if (getSystemInfo().osVersion[0] >= 8)
                {
                    baseSize.x = bounds.size.width;
                    baseSize.y = bounds.size.height;
                }
                else
                {
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
                    if (getSystemInfo().isIPadMini)
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
    
    const DisplayInfo& getDisplayInfo()
    {
        assert(display::intern::setup);
        return display::intern::displayInfo;
    }
}
