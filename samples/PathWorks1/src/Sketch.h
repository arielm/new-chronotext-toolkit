#pragma once

#include "chronotext/cinder/CinderSketch.h"
#include "chronotext/texture/TextureManager.h"
#include "chronotext/path/SplinePath.h"
#include "chronotext/path/FXGDocument.h"

class Sketch : public chr::CinderSketch
{
    chr::TextureManager textureManager;
    chr::TextureRef dotTexture;
    
    chr::SplinePath spline1;
    chr::FollowablePath path1;
    
    chr::SplinePath spline2;
    chr::FollowablePath path2;
    
    FXGDocument document;
    
public:
    Sketch(void *context, void *delegate = NULL);
    
    void setup(bool renewContext);
    void draw();
    
    void drawDots(const chr::SplinePath &spline);
};
