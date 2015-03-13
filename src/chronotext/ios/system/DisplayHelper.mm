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
        
        void Helper::setup(const system::InitInfo &initInfo)
        {
            if (!intern::setup)
            {
                auto bounds = UIScreen.mainScreen.bounds;
                auto orientation = UIApplication.sharedApplication.statusBarOrientation;
                float contentScale;

                // ---
                
                Vec2i baseSize;
                
                if ([UIScreen.mainScreen respondsToSelector:@selector(nativeScale)]) // I.E. IOS 8+
                {
                    /*
                     * TODO:
                     *
                     * 1) TEST ON IPHONE 6 AND 6+ DEVICES
                     *
                     * 2) HANDLE "DISPLAY ZOOM" MODES:
                     *    https://github.com/brackeen/glfm/blob/c3d7a72872d82eac903285b6f108ea83ac79e66c/src/glfm_platform_ios.m#L366-371
                     */
                    
                    /*
                     * ON IPHONE 6+:
                     * - DEVICE: ~2.609 (3 * 1920 / 2208)
                     * - SIMULATOR: 3
                     *
                     * OTHERWISE: EQUALS TO UIScreen.mainScreen.scale
                     */
                    contentScale = UIScreen.mainScreen.nativeScale;

                    baseSize.x = bounds.size.width;
                    baseSize.y = bounds.size.height;
                }
                else
                {
                    contentScale = UIScreen.mainScreen.scale;
                    
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
                else if (magSize == 414 * 736)
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
