/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * PLAYING "MONOCLE MAN" ANIMATION EXPORTED FROM FLASH USING THE GRAPEFRUKT EXPORTER:
 *
 * http://prototyprally.com/introducing-the-grapefrukt-asset-exporter
 * https://github.com/zaphire/Monocle-Engine/tree/master/Content/Flash
 *
 *
 * FEATURES:
 *
 * 1) WORKS ON OSX, iOS AND ANDROID
 * 2) DEMONSTRATES USAGE OF TextureAtlas
 * 3) SOLUTION TO "PREMULTIPLIED PNG CURSE" (NECESSARY ON iOS)
 * 4) AUTOMATIC TEXTURE-RELOADING AFTER "DISCARDING" (E.G. FOLLOWING OPEN-GL CONTEXT-LOSS OR MEMORY-WARNING...)
 * 5) USING SKETCH'S Clock IN ORDER TO "FREEZE TIME" WHEN APP IS IN BACKGROUND (iOS, ANDROID)
 *
 *
 * TODO:
 *
 * 1) ADAPT TO WINDOWS
 *    - PRE-REQUISITES:
 *      - EITHER: HANDLING RESOURCES VIA ASSETS
 *      - OR: ADAPTING InputSource::getSubSource() FOR WINDOWS
 */

/*
 * FOLLOW-UP:
 *
 * THIS DEMO IS PRETTY OLD...
 * NOWADAYS, "SPINE" LOOKS LIKE THE DEFINITIVE ANSWER:
 *
 * http://esotericsoftware.com
 * http://www.java-gaming.org/index.php?topic=27914.0
 */

/*
 * IN SYNC WITH develop BRANCH
 *
 * OSX: COMMIT 49c2b15
 * IOS: COMMIT 49c2b15
 * ANDROID: COMMIT 49c2b15
 */

/*
 * DESKTOP ONLY: PRESS D TO "DISCARD" TEXTURES
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"

#include "Animation.h"

class Sketch : public chr::CinderSketch
{
public:
    void setup() final;
    void draw() final;
    
    bool keyDown(const ci::app::KeyEvent &event) final;
    
protected:
    chr::TextureManager textureManager;

    Animation animation;
    float scale;
};
