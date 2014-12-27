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
 * OSX: COMMIT 81fee47
 * IOS: COMMIT 81fee47
 * ANDROID: COMMIT 81fee47
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"
#include "chronotext/texture/TextureManager.h"
#include "chronotext/font/xf/FontManager.h"
#include "chronotext/path/SplinePath.h"
#include "chronotext/path/ShapeTesselator.h"

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
    
    void drawDots(const chr::SplinePath &spline);

protected:
    chr::TextureManager textureManager;
    chr::Texture::Ref dotTexture;
    
    chr::xf::FontManager fontManager;
    std::shared_ptr<chr::XFont> font;
    
    chr::SplinePath spline;
    chr::FollowablePath path;
    
    chr::SplinePath peanutSpline;
    chr::FollowablePath peanutPath;
    
    std::shared_ptr<chr::ShapeMesh> lys;
    ci::Vec2f lysOffset;
    
    float scale;
    float offset1;
    float offset2;
};
