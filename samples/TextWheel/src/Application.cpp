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

#include "chronotext/app/cinder/CinderApp.h"

#include "Sketch.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace chr;

class Application : public CinderApp
{
public:
    Sketch *target;
    
    CinderSketch* createSketch()
    {
        target = new Sketch(this);
        return target;
    }
    
    void prepareSettings(Settings *settings)
    {
        CinderApp::prepareSettings(settings);
        settings->setWindowSize(1024, 768);
    }
    
    void keyDown(KeyEvent event)
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
};

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
