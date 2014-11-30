/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * STUB FOR RUNNING A CinderSketch ON THE DESKTOP
 */

#include "chronotext/app/cinder/CinderDelegate.h"

class Application : public chr::CinderDelegate
{
public:
    void prepareSettings(Settings *settings)
    {
        CinderDelegate::applyDefaultSettings(settings);
        
        if (loadEmulators(chr::InputSource::getResource("emulators.json")))
        {
//          emulate(settings, "IPHONE_3GS", chr::DisplayInfo::ORIENTATION_PORTRAIT);
            emulate(settings, "IPHONE_4", chr::DisplayInfo::ORIENTATION_LANDSCAPE);
//          emulate(settings, "IPHONE_5", chr::DisplayInfo::ORIENTATION_LANDSCAPE);
//          emulate(settings, "IPAD_1", chr::DisplayInfo::ORIENTATION_LANDSCAPE);
//          emulate(settings, "GALAXY_S", chr::DisplayInfo::ORIENTATION_PORTRAIT);
//          emulate(settings, "GALAXY_S4", chr::DisplayInfo::ORIENTATION_LANDSCAPE);
//          emulate(settings, "NEXUS_7_2012", chr::DisplayInfo::ORIENTATION_LANDSCAPE);
        }
    }
    
    void keyDown(ci::app::KeyEvent event)
    {
        switch (event.getCode())
        {
            case ci::app::KeyEvent::KEY_ESCAPE:
                quit();
                break;
        }
    }
};
