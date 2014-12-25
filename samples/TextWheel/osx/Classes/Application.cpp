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
    target = reinterpret_cast<Sketch*>(sketch);
}

void Application::applySettings(Settings *settings)
{
    settings->setWindowSize(1024, 768);
}

void Application::keyDown(KeyEvent event)
{
    switch (event.getCode())
    {
        case KeyEvent::KEY_ESCAPE:
            quit();
            break;
            
        case KeyEvent::KEY_RIGHT:
            target->nextVersion();
            break;
            
        case KeyEvent::KEY_LEFT:
            target->previousVersion();
            break;
    }
}
