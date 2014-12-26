/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "Application.h"
#include "Sketch.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace chr;

void Application::sketchCreated(CinderSketch *sketch)
{
    Texture::VERBOSE = true;
    TextureHelper::PROBE_MEMORY = true;

    target = reinterpret_cast<Sketch*>(sketch);
}

void Application::applySettings(Settings *settings)
{
    if (loadEmulators(chr::InputSource::getResource("emulators.json")))
    {
//      emulate(settings, "IPHONE_3GS", chr::DisplayInfo::ORIENTATION_PORTRAIT);
//      emulate(settings, "IPHONE_4", chr::DisplayInfo::ORIENTATION_LANDSCAPE);
//      emulate(settings, "IPHONE_5", chr::DisplayInfo::ORIENTATION_LANDSCAPE);
        emulate(settings, "IPAD_1", chr::DisplayInfo::ORIENTATION_LANDSCAPE);
//      emulate(settings, "GALAXY_S", chr::DisplayInfo::ORIENTATION_PORTRAIT);
//      emulate(settings, "GALAXY_S4", chr::DisplayInfo::ORIENTATION_LANDSCAPE);
//      emulate(settings, "NEXUS_7_2012", chr::DisplayInfo::ORIENTATION_LANDSCAPE);
    }
}

void Application::keyDown(KeyEvent event)
{
    switch (event.getCode())
    {
        case KeyEvent::KEY_ESCAPE:
            quit();
            break;
        
        case KeyEvent::KEY_d:
            target->textureManager.discardTextures();
            break;
    }
}
