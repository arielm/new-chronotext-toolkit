/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * DEPRECATED / IN SYNC WITH MobileLifecycleRework BRANCH
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"
#include "chronotext/texture/TextureManager.h"
#include "chronotext/path/SplinePath.h"
#include "chronotext/path/Hairline.h"

class Sketch : public chr::CinderSketch
{
    chr::TextureManager textureManager;

    chr::TextureRef roadTexture;
    chr::TextureRef checkerTexture;
    chr::TextureRef dotTexture;
    
    chr::FollowablePath roadPath;
    chr::TexturedTriangleStrip roadStrip;
    chr::TexturedTriangleStrip checkerStrip;
    
    chr::SplinePath peanutSpline;
    chr::FollowablePath peanutPath;
    chr::Hairline peanutHairline;
    
    std::vector<std::pair<chr::FollowablePath, chr::Hairline>> lys;
    ci::Vec2f lysOffset;
    
    float scale;
    float offset;
    
public:
    Sketch(void *context, void *delegate = nullptr);
    
    void setup();
    void resize();
    void update();
    void draw();
    
    void drawDotOnPath(const chr::FollowablePath &path);
};
