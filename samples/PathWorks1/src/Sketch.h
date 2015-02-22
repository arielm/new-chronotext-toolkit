/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * IN SYNC WITH develop BRANCH
 *
 * OSX: COMMIT 446e1a7
 * IOS: COMMIT 446e1a7
 * ANDROID: COMMIT 446e1a7
 */

/*
 * FIXME: "PEANUT" LINE IS NOT DRAWN ON ANDROID (REPRODUCED ON NEXUS 7, 2013)
 *
 * REASON: INSTEAD OF BEING "IGNORED", glEnable(GL_LINE_SMOOTH) IS AFFECTING THE
 * OPACITY OF LINE-VERTICES BASED ON THEIR POSITION (LOOKS LIKE SOME OPEN-GL DRIVER BUG)
 *
 * E.G. VERTICES WITH POSITIVE HORIZONTAL COORDINATES ARE NOT VISIBLE (IN BETWEEN: OPACITY-BLENDING OCCURS...)
 *
 * SOLUTION: USING SOME API FUNCTION (E.G. IN GLUtils) FOR ENABLING/DISABLING LINE-ANTIALISING
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
