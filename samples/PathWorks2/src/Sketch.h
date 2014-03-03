/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"
#include "chronotext/texture/TextureManager.h"
#include "chronotext/path/SplinePath.h"

#include "Hairline.h"

class Sketch : public chr::CinderSketch
{
    chr::TextureManager textureManager;

    chr::TextureRef roadTexture;
    chr::TextureRef dotTexture;
    
    chr::FollowablePath roadPath;
    chr::TexturedTriangleStrip roadStrip;
    
    chr::SplinePath peanutSpline;
    chr::FollowablePath peanutPath;
    Hairline peanutHairline;
    
    std::vector<std::pair<chr::FollowablePath, Hairline>> lys;
    ci::Vec2f offset;
    
    float scale;
    float position;
    
public:
    Sketch(void *context, void *delegate = NULL);
    
    void setup(bool renewContext);
    void resize();
    void update();
    void draw();
    
    void drawDotOnPath(const chr::FollowablePath &path);
    bool isHighDensity() const;
};
