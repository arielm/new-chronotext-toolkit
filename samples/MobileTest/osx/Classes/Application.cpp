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

#include "chronotext/cinder/CinderApp.h"

#include "Sketch.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace chr;

class Application : public CinderApp
{
public:
    Application();
    
    void prepareSettings(Settings *settings);
    void keyDown(KeyEvent event);
};

Application::Application()
{
    sketch = new Sketch(this);
}

void Application::prepareSettings(Settings *settings)
{
    CinderApp::prepareSettings(settings);
    
    if (loadEmulators(InputSource::getResource("emulators.json")))
    {
//      emulate(settings, "IPHONE_3GS", DisplayInfo::ORIENTATION_PORTRAIT);
        emulate(settings, "IPHONE_4", DisplayInfo::ORIENTATION_LANDSCAPE);
//      emulate(settings, "IPHONE_5", DisplayInfo::ORIENTATION_LANDSCAPE);
//      emulate(settings, "IPAD_1", DisplayInfo::ORIENTATION_LANDSCAPE);
//      emulate(settings, "GALAXY_S", DisplayInfo::ORIENTATION_PORTRAIT);
//      emulate(settings, "GALAXY_S4", DisplayInfo::ORIENTATION_LANDSCAPE);
//      emulate(settings, "NEXUS_7_2012", DisplayInfo::ORIENTATION_LANDSCAPE);
    }
}

void Application::keyDown(KeyEvent event)
{
    switch (event.getCode())
    {
        case KeyEvent::KEY_ESCAPE:
            quit();
            break;
    }
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
