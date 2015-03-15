/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * FEATURES:
 *
 * 1) DEMONSTRATE USAGE OF chr::SplinePath, chr::FollowablePath, chr::Hairline, ETC.
 * 2) SMOOTH MOTION ON A "FOLLOWABLE" PATH
 * 3) PARSING OF VECTORIAL-DATA FROM ADOBE'S FXG FILES
 * 4) RESOLUTION-INDEPENDENT "HAIRLINE" RENDERING
 * 5) COMPLEX (SINGLE AND MULTI PORTION) TRIANGLE-STRIPPING
 *
 *
 * TODO: RE-ADAPT TO OSX RETINA SCREENS
 */

/*
 * IN SYNC WITH https://github.com/arielm/new-chronotext-toolkit/tree/v4.0.0
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"
#include "chronotext/texture/TextureManager.h"
#include "chronotext/path/SplinePath.h"
#include "chronotext/path/Hairline.h"

class Sketch : public chr::CinderSketch
{
public:
    void setup() final;
    void resize() final;
    void update() final;
    void draw() final;
    
protected:
    chr::TextureManager textureManager;
    
    chr::Texture::Ref roadTexture;
    chr::Texture::Ref checkerTexture;
    chr::Texture::Ref dotTexture;
    
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
    
    void drawDotOnPath(const chr::FollowablePath &path);
};
