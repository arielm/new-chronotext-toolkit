/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * FEATURES:
 *
 * 1) WORKS ON OSX
 * 2) DEMONSTRATES HOW TO WORK WITH THE ZFont SYSTEM (UNICODE TEXT RENDERING)
 * 3) WRITING ON A SPIRAL
 * 4) USING A FontSequence (OPTIMAL FOR STATIC TEXT)
 *
 *
 * INSTRUCTIONS:
 *
 * PRESS RIGHT OR LEFT TO SWITCH BETWEEN LANGUAGES
 *
 *
 * TODO:
 *
 * 1) TWEAK THE FONT-SIZES SO THAT ALL OF THEM SHARE A "SIMILAR" HEIGHT
 *
 * 2) ADAPT TO iOS AND ANDROID:
 *    ONLY A MATTER OF PICKING THE RIGHT SET OF FONTS
 *
 *
 * ADDITIONAL REQUIREMENTS:
 *
 * 1) hb-icu BLOCK
 *    https://github.com/arielm/hb-icu
 *
 * 2) Freetype BLOCK:
 *    https://github.com/arielm/Freetype
 *
 *
 * REFERENCES:
 *
 * 1) https://github.com/arielm/Unicode
 *
 * 2) http://www.wordproject.org/bibles
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"
#include "chronotext/font/zf/FontManager.h"

#include "TextSpiral.h"

class Sketch : public chr::CinderSketch
{
    chr::zf::FontManager fontManager;

    std::shared_ptr<chr::ZFont> font;
    TextSpiral spiral;

    std::vector<std::string> languages;
    std::map<std::string, std::unique_ptr<chr::zf::LineLayout>> layouts;
    std::map<std::string, chr::zf::FontSequence> sequences;

    float scale;
    float rotation;
    float direction;
    int currentLangIndex;

public:
    Sketch(void *context, void *delegate = NULL);
    
    void setup(bool renewContext);
    void event(int id);
    void resize();
    void update();
    void draw();
    
    void addVersion(const std::string &lang);
    void nextVersion();
    void previousVersion();
};
