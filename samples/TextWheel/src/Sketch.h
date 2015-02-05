/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * IN SYNC WITH develop BRANCH / COMMIT 1b924b1
 */

/*
 * FEATURES:
 *
 * 1) DEMONSTRATES HOW TO WORK WITH THE ZFont SYSTEM:
 *    - WRITING IN 12 DIFFERENT LANGUAGES USING A SINGLE VIRTUAL-FONT
 *
 * 2) WRITING ON A SPIRAL:
 *    - OBTAINING THE LOCATION OF A POINT (2D) GIVEN IT'S POSITION (1D) ON A SPIRAL IS A SIMPLE LINEAR-OPERATION
 *
 * 3) USING A FontSequence:
 *    - OPTIMAL FOR STATIC TEXT (A VERTEX-ARRAY OBJECT IS GENERATED)
 *
 *
 * INSTRUCTIONS:
 *
 * 1) PRESS THE RIGHT OR LEFT KEYS TO SWITCH BETWEEN LANGUAGES
 *
 * 2) PRESS U TO "UNLOAD" THE FONT-MANAGER:
 *    - FROM THIS POINT: RESOURCES WILL BE RELOADED AND TEXTURES RECREATED ONLY WHEN NECESSARY
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
 * TODO:
 *
 * 1) ADAPT TO iOS, ANDROID AND WINDOWS:
 *    - REQUIRES PICKING THE RIGHT SET OF FONTS FOR EACH PLATFORM
 *    - FOR WINDOWS: REQUIRES BUILDING HARFBUZZ AND ICU
 *
 *
 * REFERENCES:
 *
 * 1) https://github.com/arielm/Unicode
 * 2) http://www.wordproject.org/bibles
 * 3) http://chronotext.org/Isaiah
 *
 * WE'RE USING GENESIS 11:1, THE BIBLICAL PASSAGE MENTIONING THE TOWER OF BABEL:
 * - THE TEXT IS OBVIOUSLY ON PURPOSE WHEN DEALING WITH THE MULTIPLICITY OF LANGUAGES
 * - BESIDES, THE BIBLE IS THE ONLY TEXT-SOURCE AVAILABLE IN SUCH A VARIETY OF LANGUAGES
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"
#include "chronotext/font/zf/FontManager.h"

#include "TextSpiral.h"

class Sketch : public chr::CinderSketch
{
public:
    void setup() final;
    void resize() final;
    
    void update() final;
    void draw() final;

    bool keyDown(const ci::app::KeyEvent &event) final;
    
protected:
    chr::zf::FontManager fontManager;
    std::shared_ptr<chr::ZFont> font;
    
    TextSpiral spiral;
    
    std::vector<std::string> languages;
    std::map<std::string, std::unique_ptr<chr::zf::LineLayout>> layouts;
    std::map<std::string, std::unique_ptr<chr::zf::FontSequence>> sequences;
    
    float scale;
    float rotation;
    float direction;
    int currentLangIndex;

    void addVersion(const std::string &lang);
    void nextVersion();
    void previousVersion();
};
