/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * PLAYING "MONOCLE MAN" ANIMATION EXPORTED FROM FLASH USING THE GRAPEFRUKT EXPORTER:
 * http://prototyprally.com/introducing-the-grapefrukt-asset-exporter
 * https://github.com/zaphire/Monocle-Engine/tree/master/Content/Flash
 *
 *
 * FEATURES:
 *
 * 1) WORKS ON OSX, iOS AND ANDROID
 * 2) DEMONSTRATES USAGE OF TextureAtlas
 * 3) DEMONSTRATES SOLUTION TO "PREMULTIPLIED PNG CURSE" (NECESSARY ON iOS)
 * 4) DEMONSTRATES AUTOMATIC TEXTURE RELOADING AFTER OPEN-GL CONTEXT-LOSS (NECESSARY ON ANDROID)
 * 5) DEMONSTRATES USAGE OF Clock IN ORDER TO "FREEZE TIME" WHEN APP IS IN BACKGROUND (iOS, ANDROID)
 *
 *
 * TODO:
 *
 * 1) ADAPT TO WINDOWS
 *    - PRE-REQUISITE:
 *      - EITHER: HANDLING RESOURCES VIA ASSETS
 *      - OR: ADAPTING InputSource::getSubSource() FOR WINDOWS
 *
 *
 * FOLLOW-UP:
 *
 * THIS DEMO IS PRETTY OLD. NOWADAYS, "SPINE" LOOKS LIKE THE DEFINITIVE ANSWER:
 * http://esotericsoftware.com
 * http://www.java-gaming.org/index.php?topic=27914.0
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"
#include "chronotext/time/Clock.h"

#include "Animation.h"

class Sketch : public chr::CinderSketch
{
    chr::TextureManager textureManager;
    Animation animation;
    
    float scale;
    chr::Clock clock;

public:
    Sketch(void *context, void *delegate = NULL);
    
    void setup(bool renewContext);
    void start(int flags);
    void stop(int flags);
    void draw();
};
