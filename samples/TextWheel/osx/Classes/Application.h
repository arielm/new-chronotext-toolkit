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
    void applySettings(Settings *settings) override;
    void sketchCreated(chr::CinderSketch *sketch) override;
    void keyDown(ci::app::KeyEvent event) override;
    
protected:
    Sketch *target;
};
