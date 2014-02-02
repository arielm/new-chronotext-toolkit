/*
 * PLAYING "MONOCLE MAN" ANIMATION EXPORTED FROM FLASH USING THE GRAPEFRUKT EXPORTER:
 * http://prototyprally.com/introducing-the-grapefrukt-asset-exporter
 * https://github.com/zaphire/Monocle-Engine/tree/master/Content/Flash
 *
 *
 * FEATURES:
 *
 * 1) DEMONSTRATES USAGE OF TextureAtlas
 * 2) DEMONSTRATES AUTOMATIC TEXTURE RELOADING AFTER OPEN-GL CONTEXT-LOSS
 *
 *
 * TODO:
 *
 * 1) ADAPT TO WINDOWS
 *    - PRE-REQUISITE:
 *      - EITHER: HANDLING RESOURCES VIA ASSETS
 *      - OR: ADAPTING InputSource::getSubSource() WINDOWS
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"

#include "Animation.h"

class Sketch : public chr::CinderSketch
{
    chr::TextureManager textureManager;
    std::unique_ptr<Animation> animation;
    float scale;

public:
    Sketch(void *context, void *delegate = NULL);
    
    void setup(bool renewContext);
    void resize();
    void draw();
};
