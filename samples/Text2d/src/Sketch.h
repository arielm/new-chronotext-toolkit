/*
 * ...
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"

class Sketch : public chr::CinderSketch
{
public:
    Sketch(void *context, void *delegate = NULL);
    
    void setup(bool renewContext);
    void draw();
};
