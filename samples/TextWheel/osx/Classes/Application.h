/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * STUB FOR RUNNING A CinderSketch ON THE DESKTOP
 */

#pragma once

#include "chronotext/app/cinder/CinderDelegate.h"

class Sketch;

class Application : public chr::CinderDelegate
{
public:
    void sketchCreated(chr::CinderSketch *sketch) override
    {
        target = reinterpret_cast<Sketch*>(sketch);
    }
    
    void applySettings(Settings *settings) override
    {
        settings->setWindowSize(1024, 768);
    }
    
    void keyDown(ci::app::KeyEvent event) override
    {
        switch (event.getCode())
        {
            case ci::app::KeyEvent::KEY_ESCAPE:
                quit();
                break;
                
//            case ci::app::KeyEvent::KEY_RIGHT:
//                target->nextVersion();
//                break;
//                
//            case ci::app::KeyEvent::KEY_LEFT:
//                target->previousVersion();
//                break;
        }
    }
    
protected:
    Sketch *target;
};
