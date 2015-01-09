/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/app/system/DisplayHelper.h"
#include "chronotext/Context.h"

#include "cinder/app/Renderer.h"

using namespace std;
using namespace ci;
using namespace ci::app;

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
                intern::displayInfo = initInfo.getDisplayInfo();
                
                // ---
                
                LOGI_IF(true) << "DISPLAY INFO: " << intern::displayInfo << endl; // LOG: VERBOSE
                
                intern::setup = true;
            }
        }
        
        void Helper::shutdown()
        {
            intern::setup = false;
        }
        
        // ---
        
        int Helper::getAALevel(AppNative *application)
        {
            auto renderer = static_pointer_cast<RendererGl>(application->getRenderer());
            return renderer->getAntiAliasing(); // XXX: THIS IS *NOT* RETURNING ENUM-VALUES LIKE RendererGl::AA_MSAA_4
        }
    }
    
    const DisplayInfo& getDisplayInfo()
    {
        assert(display::intern::setup);
        return display::intern::displayInfo;
    }
}
