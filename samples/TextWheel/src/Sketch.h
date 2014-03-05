/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"
#include "chronotext/font/zf/FontManager.h"

#include "TextSpiral.h"

class Sketch : public chr::CinderSketch
{
    chr::zf::FontManager fontManager;

    float scale;
    float position;

    std::shared_ptr<chr::ZFont> font;
    std::vector<std::string> languages;
    std::map<std::string, std::vector<std::unique_ptr<chr::zf::LineLayout>>> lines;
    
    int currentLangIndex;
    int currentLineIndex;
    
    TextSpiral spiral;
    
public:
    Sketch(void *context, void *delegate = NULL);
    
    void setup(bool renewContext);
    void event(int id);
    void resize();
    void update();
    void draw();
    
    void addVersion(const std::string &lang);
    std::unique_ptr<chr::zf::LineLayout>& getNextLine();
};
