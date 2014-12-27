/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * IN SYNC WITH ResReWork BRANCH
 *
 * OSX: COMMIT b33c7e3
 * IOS: COMMIT b33c7e3
 * ANDROID: COMMIT b33c7e3
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"
#include "chronotext/texture/TextureManager.h"
#include "chronotext/path/SplinePath.h"
#include "chronotext/path/Hairline.h"

class Sketch : public chr::CinderSketch
{
public:
    Sketch()
    :
    CinderSketch()
    {}
    
    void setup() final;
    void resize() final;
    void update() final;
    void draw() final;
    
    void drawDotOnPath(const chr::FollowablePath &path);
    
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
};
