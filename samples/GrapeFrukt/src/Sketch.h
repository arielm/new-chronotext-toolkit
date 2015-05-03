/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * PLAYING "MONOCLE MAN" ANIMATION EXPORTED FROM FLASH USING THE" GRAPEFRUKT" EXPORTER:
 *
 * http://prototyprally.com/introducing-the-grapefrukt-asset-exporter
 * https://github.com/zaphire/Monocle-Engine/tree/master/Content/Flash
 *
 *
 * FEATURES:
 *
 * 1) DEMONSTRATES USAGE OF chr::TextureAtlas, ETC.
 * 2) SOLUTION TO "PREMULTIPLIED PNG CURSE" (NECESSARY ON iOS)
 * 3) AUTOMATIC TEXTURE-RELOADING AFTER "DISCARDING" (E.G. FOLLOWING OPEN-GL CONTEXT-LOSS OR MEMORY-WARNING...)
 * 4) USING THE SKETCH'S Clock IN ORDER TO "FREEZE" TIME WHILE UNDERLYING APP IS IN THE BACKGROUND
 *
 *
 * TODO:
 *
 * 1) ADAPT chr::TextureAtlas TO WINDOWS
 *    - PRE-REQUISITES:
 *      - EITHER: HANDLING RESOURCES VIA ASSETS
 *      - OR: ADAPTING InputSource::getSubSource() FOR WINDOWS
 *
 * 2) TEXTURES SHOULD BE RENDERED VIA chr::QuadMatrix AND chr::QuadBatch
 *    - PRE-REQUISITE: TEXTURE-BATCHING SUPPORT AT THE TOOLKIT'S LEVEL
 */

/*
 * FOLLOW-UP: THIS DEMO IS PRETTY OUTDATED...
 *
 * 1) THE "GRAPEFRUKT" EXPORTER WAS NOT PRACTICAL ANYWAY IN TERM OF DESIGNER-TO-PROGRAMMER PIPELINE
 *
 * 2) NOWADAYS, "SPINE" LOOKS LIKE THE DEFINITIVE ANSWER:
 *    - http://www.java-gaming.org/index.php?topic=27914.0
 *    - http://esotericsoftware.com
 */

/*
 * IN SYNC WITH https://github.com/arielm/new-chronotext-toolkit/tree/develop
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
};
